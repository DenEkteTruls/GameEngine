#include "game.h"

int main(int argc, char **argv)
{
	Engine engine("game", 800, 600);
	GameObject player(engine.renderer, "player.png", 225, 224);
	GameObject rhodes(engine.renderer, "rhodes.png", 512, 512);
	GameObject grass(engine.renderer, "C:/Users/cas1710/Downloads/gress.png", 64, 64);

	engine.addGameObject(&player);
	engine.addGameObject(&rhodes);

	player.dst.x = 350;
	player.dst.y = 200;

	while (engine.running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			engine.eventHandler(event);
			rhodes.eventHandler_controllable(event);
		}

		engine.setBackgroundColor(255, 255, 255);
		engine.renderClear();

		engine.collide_preventer(rhodes.id);

		engine.renderGameObjects();
		engine.drawMapSection(grass, "horizontal", 2, 8, 8);
		engine.render();
	}

	engine.kill();
	return 0;
}