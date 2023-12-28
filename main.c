#include <SDL.h>
#include <SDL_image.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include "ulist.h"

#define DEFAULT_WIDTH 600;
#define FPS 60

#define RUNNER_ACCELERATION 0.001
#define RUNNER_BG_CLOUD_SPEED 0.2
#define RUNNER_BOTTOM_PAD 10
#define RUNNER_CLEAR_TIME 3000
#define RUNNER_CLOUD_FREQUENCY 0.5
#define RUNNER_GAMEOVER_CLEAR_TIME 750
#define RUNNER_GAP_COEFFICIENT 0.6
#define RUNNER_GRAVITY 0.6
#define RUNNER_INITIAL_JUMP_VELOCITY 12
#define RUNNER_INVERT_FADE_DURATION 12000
#define RUNNER_INVERT_DISTANCE 700
#define RUNNER_MAX_BLINK_COUNT 3
#define RUNNER_MAX_CLOUDS 6
#define RUNNER_MAX_OBSTACLE_LENGTH 3
#define RUNNER_MAX_OBSTACLE_DUPLICATION 2
#define RUNNER_MAX_SPEED 13
#define RUNNER_MIN_JUMP_HEIGHT 35
#define RUNNER_MOBILE_SPEED_COEFFICIENT 1.2
#define RUNNER_SPEED 6
#define RUNNER_SPEED_DROP_COEFFICIENT 3

SDL_Renderer *renderer;

//typedef struct {
//	//
//} DistanceMeter;

typedef struct {
	int width;
	int height;
	// trex
	// distancemeter
	int distanceRan;
	int highestScore;
	int time;
	int runningTime;
	int msPerFrame;
	int currentSpeed;
	Ulist *obstacles;
	bool activated;
	bool playing;
	bool crashed;
	bool paused;
	bool inverted;
	bool invertTimer;
	// resizeTimerId_
	int playCount;
	// soundFx
	// audioContext
	SDL_Texture* spriteAtlas;
	// images
	// imagesLoaded
} Runner;

Runner runner; // singletone

void runnerLoadImages() {
	runner.spriteAtlas = IMG_LoadTexture(renderer, "assets/sprites100.png");
	//int img_w, img_h;
	//SDL_QueryTexture(runner.spriteAtlas, NULL, NULL, &img_w, &img_h); // get the width and height of the texture
}

void runnerAdjustDimensions() {
	runner.width = DEFAULT_WIDTH;
	// distance meter ...
}

void runnerInit() {
	runner.distanceRan = 0;
	runner.highestScore = 0;
	runner.time = 0;
	runner.msPerFrame = 1000 / FPS;
	//runner.currentSpeed = ;
	runner.obstacles = ulist_create();
	runner.activated = false;
	runner.playing = false;
	runner.crashed = false;
	runner.paused = false;
	runner.inverted = false;
	runner.invertTimer = 0;
	runner.playCount = 0;
	runnerLoadImages();
	runnerAdjustDimensions();
}

void blitAtlasImage(int atlasX, int atlasY, int destX, int destY, int w, int h, bool center) {
	SDL_Rect dest = { destX, destY, w, h }, src = {atlasX, atlasY, w, h};
	if (center) {
		dest.x -= (dest.w / 2);
		dest.y -= (dest.h / 2);
	}
	SDL_RenderCopy(renderer, runner.spriteAtlas, &src, &dest);
}

int main(int argc, char* args[]) {
	//ulist_test();

	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	//Create window
	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1300, 480, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	runnerInit();

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
		// clear the screen
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		// copy the texture to the rendering context
		/*SDL_Rect rect = { .x = 0, .y = 100, .w = 25, .h = 50 };
		SDL_Rect rect2 = { .x = 332, .y = 2, .w = 25, .h = 50 };
		SDL_RenderCopy(renderer, runner.spriteAtlas, &rect2, &rect);
		*/
		blitAtlasImage(332, 2, 50, 100, 25, 50, false);
		blitAtlasImage(332, 2, 50, 100, 25, 50, true);
		// flip the backbuffer
		// this means that everything that we prepared behind the screens is actually shown
		SDL_RenderPresent(renderer);
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
