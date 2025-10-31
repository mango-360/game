#include "World.h"

#undef main

World world;

int main(int argc, char* argv[])
{
	world.init();

	while (world.isRunning())
	{
		world.run();
	}
	
	return 0;
}