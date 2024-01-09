#include <SDL.h>
#include <SDL_image.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "misc.h"
#include "graphics.h"
#include "distance_meter.h"
#include "cloud.h"
#include "obstacle.h"
#include "horizon_line.h"
#include "trex.h"
#include "runner.h"

#pragma warning(disable:4996)

int main(int argc, char* args[]) {
	srand((unsigned int)time(NULL)); // Seed the random number generator

	graphicsInit();

	runnerInit();
	
	SDL_Event event;
	bool quit = false;
	while (quit == false) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				//printf("DOWN%d ", event.key.keysym.sym & 0xFF);
				runnerOnKeyDown(event.key.keysym.sym & 0xFF);
				break;
			case SDL_KEYUP:
				//printf("UP%d ", event.key.keysym.sym & 0xFF);
				runnerOnKeyUp(event.key.keysym.sym & 0xFF);
				break;
			default:
				break;
			}
		}
		runnerUpdate();
		SDL_Delay(20);		
	}

	graphicsDestroy();

	return 0;
}
