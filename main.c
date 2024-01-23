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
		int frameStartTime = getTimeStamp();
		//printf("frameStartTime = %d\n", frameStartTime);
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
		if (runner.nextUpdateScheduled) {
			runnerUpdate();
		}

		int frameTime = getTimeStamp() - frameStartTime;
#define FRAME_TIME 16
		if (frameTime < FRAME_TIME) { // 1000ms/60frames = 16.(6)
			// printf("frameTime = %d\n", frameTime);
			graphicsDelay(FRAME_TIME - frameTime);
		}
	}

	graphicsDestroy();

	return 0;
}
