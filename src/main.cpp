#include "World.h"

#undef main

World world;

int main(int argc, char* argv[])
{
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	world.init();

	while (world.isRunning())
	{
		frameStart = SDL_GetTicks();

		world.run();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}
	
	return 0;
}