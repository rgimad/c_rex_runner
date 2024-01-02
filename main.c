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
#include "horizon_line.h"
#include "runner.h"

#pragma warning(disable:4996)

int main(int argc, char* args[]) {
	srand((unsigned int)time(NULL)); // Seed the random number generator

	//ulist_test();

	graphicsInit();

	runnerInit();

	extern SDL_Renderer* renderer;
	SDL_SetRenderDrawColor(renderer, 0xF7, 0xF7, 0xF7, 0xFF);
	SDL_RenderClear(renderer);

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

	SDL_RenderPresent(renderer);

	//Hack to get window to stay up
	SDL_Event e;
	bool quit = false;
	while (quit == false) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
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

	extern SDL_Window* window;//
	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
