#pragma once

#include "game.h"


//////////////////////		GameObject		////////////////////////


typedef struct GameObject
{
	int id;
	int speed = 5;
	static int current_id;
	bool collided = false;
	int collide_info[4]{ 0, 0, 0, 0 };

	SDL_Rect src{ 0, 0, 0, 0 };
	SDL_Rect dst{ 0, 0, BLOCK_SIZE, BLOCK_SIZE };
	SDL_Texture *texture;

	void eventHandler_controllable(SDL_Event event);

	GameObject(SDL_Renderer *renderer, const char *filename, int w, int h);
	~GameObject() {}
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
	switch (event.type)
	{
	case SDL_KEYDOWN:
		std::cout << !collided << " || " << !collide_info[1] << std::endl;  
		switch (event.key.keysym.scancode)
		{
		case SDL_SCANCODE_W:	if (!collided && !collide_info[0]) { dst.y -= speed; }	break;
		case SDL_SCANCODE_S:	if (!collided && !collide_info[1]) { dst.y += speed; }	break;
		case SDL_SCANCODE_A:	if (!collided && !collide_info[2]) { dst.x -= speed; }	break;
		case SDL_SCANCODE_D:	if (!collided && !collide_info[3]) { dst.x += speed; }	break;
		}
	}
	collided = false;
}
