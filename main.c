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

// Distance meter configurations
#define DM_MAX_DISTANCE_UNITS 5
#define DM_ACHIEVEMENT_DISTANCE 100
#define DM_COEFFICIENT 0.025
#define DM_FLASH_DURATION 1000/4
#define DM_FLASH_ITERATIONS 3
#define DM_WIDTH 10
#define DM_HEIGHT 13
#define DM_DEST_WIDTH 11

#define ATLAS_CACTUS_LARGE_X 332
#define ATLAS_CACTUS_LARGE_Y 2
#define ATLAS_CACTUS_SMALL_X 228
#define ATLAS_CACTUS_SMALL_Y 2
#define ATLAS_CLOUD_X 86
#define ATLAS_CLOUD_Y 2
#define ATLAS_HORIZON_X 2
#define ATLAS_HORIZON 54
#define ATLAS_MOON_X 484
#define ATLAS_MOON_Y 2
#define ATLAS_PTERODACTYL_X 134
#define ATLAS_PTERODACTYL_Y 2
#define ATLAS_RESTART_X 2
#define ATLAS_RESTART_Y 2
#define ATLAS_TEXT_SPRITE_X 655
#define ATLAS_TEXT_SPRITE_Y 2
#define ATLAS_TREX_X 848
#define ATLAS_TREX_Y 2
#define ATLAS_STAR_X 645
#define ATLAS_STAR_Y 2

SDL_Renderer *renderer;

void blitAtlasImage(int atlasX, int atlasY, int destX, int destY, int w, int h, bool center);

struct {
	//int spritePos;s
	int x;
	int y;
	int currentDistance;
	int maxScore;
	int highScore;
	int digits[DM_MAX_DISTANCE_UNITS];
	bool achievement;
	// defaultString
	int flashTimer;
	int flashIterations;
	bool invertTrigger;
	int maxScoreUnits;
} distanceMeter;

void distanceMeterInit(int w);
void distanceMeterCalcXPos(int w);
void distanceMeterDraw(int digitPos, int value, bool opt_highscore);
int distanceMeterGetActualDistance(int distance);

void distanceMeterInit(int w) {
	distanceMeter.x = 0;
	distanceMeter.y = 5;
	distanceMeter.currentDistance = 0;
	distanceMeter.maxScore = 0;
	distanceMeter.highScore = 0;
	distanceMeter.achievement = false;
	distanceMeter.flashTimer = 0;
	distanceMeter.flashIterations = 0;
	distanceMeter.invertTrigger = false;
	distanceMeter.maxScoreUnits = DM_MAX_DISTANCE_UNITS;
	// init();
	distanceMeterCalcXPos(w);
	for (int i = 0; i < distanceMeter.maxScoreUnits; i++) {
		distanceMeterDraw(i, 0, false);
	}
	distanceMeter.maxScore = (int)pow(10, distanceMeter.maxScoreUnits) - 1;
}

void distanceMeterCalcXPos(int w) {
	distanceMeter.x = w - (DM_DEST_WIDTH * (distanceMeter.maxScoreUnits + 1));
}

void distanceMeterDraw(int digitPos, int value, bool opt_highscore) {
	int dx, dy;
	if (opt_highscore) {
		dx = distanceMeter.x - (distanceMeter.maxScoreUnits * 2) * DM_WIDTH;
		dy = distanceMeter.y;
	}
	else {
		dx = distanceMeter.x;
		dy = distanceMeter.y;
	}
	blitAtlasImage(
		DM_WIDTH*value + ATLAS_TEXT_SPRITE_X,
		0 + ATLAS_TEXT_SPRITE_Y,
		digitPos*DM_DEST_WIDTH + dx,
		distanceMeter.y + dy,
		DM_WIDTH,
		DM_HEIGHT,
		false
	);
}

int distanceMeterGetActualDistance(int distance) {
	return distance ? (int)round(distance * DM_COEFFICIENT) : 0;
}



struct {
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
} runner; // singletone

void runnerInit();
void runnerAdjustDimensions();
void runnerLoadImages();

void blitAtlasImage(int atlasX, int atlasY, int destX, int destY, int w, int h, bool center) {
	SDL_Rect dest = { destX, destY, w, h }, src = { atlasX, atlasY, w, h };
	if (center) {
		dest.x -= (dest.w / 2);
		dest.y -= (dest.h / 2);
	}
	SDL_RenderCopy(renderer, runner.spriteAtlas, &src, &dest);
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
	// fill canvas 
	// horizon create
	distanceMeterInit(runner.width);
}


void runnerLoadImages() {
	runner.spriteAtlas = IMG_LoadTexture(renderer, "assets/sprites100.png");
	//int img_w, img_h;
	//SDL_QueryTexture(runner.spriteAtlas, NULL, NULL, &img_w, &img_h); // get the width and height of the texture
}

void runnerAdjustDimensions() {
	runner.width = DEFAULT_WIDTH;
	// distance meter ...
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
	window = SDL_CreateWindow("C Rex Runner", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1300, 480, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	runnerInit();

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

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
