#include <iostream>
#include "Engine3D.h"

int main(int argc, char** args) {
	Engine3D engine3d;
	bool status = engine3d.init();

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double delta = 0;

	while (true)
	{
		//Calculate delta time
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		delta = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());


		// Get the next event
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) break;
		}

		engine3d.draw(delta);
	}

	return status;
}