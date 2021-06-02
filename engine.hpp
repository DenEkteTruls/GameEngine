#pragma once

#include "game.h"

#define BLOCK_SIZE 64


typedef struct GameObject
{
	int id;
	int speed = 5;
	static int current_id;
	bool movement[4]{ 0, 0, 0, 0 };
	bool collided = false;

	SDL_Rect src{ 0, 0, 0, 0 };
	SDL_Rect dst{ 0, 0, BLOCK_SIZE, BLOCK_SIZE };
	SDL_Texture *texture;

	void eventHandler_controllable(SDL_Event event);

	GameObject(SDL_Renderer *renderer, const char *filename, int w, int h);
} _GameObjectT;


int GameObject::current_id = 0;


GameObject::GameObject(SDL_Renderer *renderer, const char *filename, int w, int h)
{
	id = current_id++;
	src.w = w; src.h = h;

	SDL_Surface *tmp = IMG_Load(filename);
	this->texture = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
}


void GameObject::eventHandler_controllable(SDL_Event event)
{
	if (!collided)
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.scancode)
			{
			case SDL_SCANCODE_W:	dst.y -= speed;	break;
			case SDL_SCANCODE_S:	dst.y += speed;	break;
			case SDL_SCANCODE_A:	dst.x -= speed;	break;
			case SDL_SCANCODE_D:	dst.x += speed;	break;
			}
		}
	}
}



//////////////////////		ENGINE		////////////////////////


class Engine
{
public:
	int width, height;
	const char *title;
	bool running = true;

	SDL_Window *window;
	SDL_Renderer *renderer;
	Error_handler error_handler;

	void collide_preventer(int id);
	void eventHandler(SDL_Event event);
	void addGameObject(_GameObjectT *gameObject);
	void setBackgroundColor(int r, int g, int b);
	void renderGameObject(_GameObjectT gameObject);
	void renderGameObjects();
	void drawMapSection(_GameObjectT block, const char *orientation, int start, int end, int height);
	void renderClear();
	void render();
	void kill();

	std::vector<_GameObjectT*> getGameObjects() { return gameObjects; }
	Engine(const char *title, int width, int height);

private:
	std::vector<_GameObjectT*> gameObjects;
};


Engine::Engine(const char *title, int width, int height)
{
	this->title = title;
	this->width = width;
	this->height = height;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "error on: SDL_INIT_EVERYTHING" << std::endl;
		_ErrorT error("SDL_INIT", "Cannot init with flag: SDL_INIT_EVERYTHING");
		error_handler.report(error);
		exit(-1);
	};

	if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) {
		std::cout << "error on: IMG_SDL_INIT_PNG" << std::endl;
		_ErrorT error("IMG_INIT", "Cannot init with flag: IMG_INIT_PNG|IMG_INIT_JPG");
		error_handler.report(error);
		exit(-1);
	}

	window = SDL_CreateWindow(this->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}


void Engine::collide_preventer(int id)
{
	_GameObjectT *currentGameObject = nullptr;

	for (_GameObjectT* gameObject : gameObjects)
	{
		if ((*gameObject).id == id) {
			currentGameObject = gameObject;
		}
	}

	for (_GameObjectT* gameObject : gameObjects)
	{
		std::cout << currentGameObject->dst.x << std::endl;
		if (currentGameObject->dst.x > gameObject->dst.x && currentGameObject->dst.x < gameObject->dst.x) {
			if (currentGameObject->dst.y > gameObject->dst.y && currentGameObject->dst.y < gameObject->dst.y) {
				currentGameObject->collided = true;
				std::cout << "COLLIDE" << std::endl;
			}
			else {
				currentGameObject->collided = false;
			}
		} else{
			currentGameObject->collided = false;
		}
	}
}


void Engine::eventHandler(SDL_Event event)
{
	switch (event.type)
	{
	case SDL_QUIT: running = false;	break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.scancode)
		{
		case SDL_SCANCODE_ESCAPE:	running = false;	break;
		}
	}
}


void Engine::addGameObject(_GameObjectT *gameObject)
{
	gameObjects.push_back(gameObject);
}


void Engine::renderGameObject(_GameObjectT gameObject)
{
	SDL_RenderCopy(renderer, gameObject.texture, &gameObject.src, &gameObject.dst);
}


void Engine::renderGameObjects()
{
	for (_GameObjectT* gameObject : gameObjects)
	{
		SDL_RenderCopy(renderer, (*gameObject).texture, &(*gameObject).src, &(*gameObject).dst);
	}
}


void Engine::setBackgroundColor(int r, int g, int b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}


void Engine::renderClear()
{
	SDL_RenderClear(renderer);
}


void Engine::render()
{
	SDL_RenderPresent(renderer);
}


void Engine::drawMapSection(_GameObjectT block, const char *orientation, int start, int end, int height)
{
	if (orientation == "horizontal")
	{
		block.dst.y = height * BLOCK_SIZE;
		for (int i = start; i < end; i++)
		{
			SDL_RenderCopy(renderer, block.texture, &block.src, &block.dst);
			block.dst.x += BLOCK_SIZE;
		}
	}
	else if (orientation == "vertical")
	{
		for (int i = start; i < end; i++)
		{
			SDL_RenderCopy(renderer, block.texture, &block.src, &block.dst);
			block.dst.y += BLOCK_SIZE;
		}
	}
	else {
		_ErrorT error("engine", "Cannot draw map with unknown orientation!");
		error_handler.report(error);
	}
}


void Engine::kill()
{
	std::cout << "Engine is shutting down man!" << std::endl;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}