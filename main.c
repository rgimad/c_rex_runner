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

	//ulist_test();

	graphicsInit();

	runnerInit();

	/* // TESTS:
	graphicsFillBackground(0xF7, 0xF7, 0xF7);

	// just test
	horizonLineInit();
	Cloud c;
	for (int i = 0; i < 15; i++) {
		cloudInit(&c, runner.width);
		c.xPos = i * 60;
		cloudDraw(&c);
	}
	 
	// test distance meter:
	distanceMeterSetHighScore(5656);
	distanceMeterReset();

	Obstacle ob;
	obstacleInit(&ob, &obstacleTypeConfigs[0], runner.width, 0, 0, -500);
	obstacleDraw(&ob);

	Obstacle ob1;
	obstacleInit(&ob1, &obstacleTypeConfigs[1], runner.width, 0, 0, -400);
	obstacleDraw(&ob1);

	Obstacle ob2;
	obstacleInit(&ob2, &obstacleTypeConfigs[2], runner.width, 0, 0, -300);
	obstacleDraw(&ob2);

	trexInit();
	*/

	graphicsRender();

	//Hack to get window to stay up
	SDL_Event event;
	bool quit = false;
	while (quit == false) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				printf("DOWN%d ", event.key.keysym.sym & 0xFF);
				break;
			case SDL_KEYUP:
				printf("UP%d ", event.key.keysym.sym & 0xFF);
				break;
			default:
				break;
			}
		}
		/*
		// clear the screen
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		// copy the texture to the rendering context
		//SDL_Rect rect = { .x = 0, .y = 100, .w = 25, .h = 50 };
		//SDL_Rect rect2 = { .x = 332, .y = 2, .w = 25, .h = 50 };
		//SDL_RenderCopy(renderer, runner.spriteAtlas, &rect2, &rect);
		//
		blitAtlasImage(332, 2, 50, 100, 25, 50, false);
		blitAtlasImage(332, 2, 50, 100, 25, 50, true);
		
		// score
		for (int i = 0; i <= 3; i++) {
			distanceMeterDraw(i, i % 10, false);
		}
		// high score:
		for (int i = 3; i >= 0; i--) {
			distanceMeterDraw(3-i, i % 10, true);
		}
		
		// flip the backbuffer
		// this means that everything that we prepared behind the screens is actually shown
		SDL_RenderPresent(renderer);
		*/
		
	}

	graphicsDestroy();

	return 0;
}
