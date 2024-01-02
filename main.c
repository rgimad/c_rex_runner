#include <SDL.h>
#include <SDL_image.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ulist.h"

#pragma warning(disable:4996)

int getRandomNumber(int _min, int _max) {
	return rand() % (_max - _min + 1) + _min;
}

void intToStr(int num, int ndigits, char* result) {
	char num_str[16]; // 16 more than enough for int
	sprintf(num_str, "%d", num); // Convert num to a string
	if (strlen(num_str) > ndigits) {
		// Copy only the last ndigits to result
		strcpy(result, num_str + strlen(num_str) - ndigits);
	}
	else {
		// Pad the string with leading zeros until it reaches a length of ndigits
		int z = ndigits - strlen(num_str);
		for (int i = 0; i < z; i++) {
			result[i] = '0';
		}
		strcpy(result + z, num_str);
	}
}


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

#define CLOUD_WIDTH 46
#define CLOUD_HEIGHT 14
#define CLOUD_MAX_GAP 400
#define CLOUD_MAX_SKY_LEVEL 30
#define CLOUD_MIN_GAP 100
#define CLOUD_MIN_SKY_LEVEL 71

#define ATLAS_CACTUS_LARGE_X 332
#define ATLAS_CACTUS_LARGE_Y 2
#define ATLAS_CACTUS_SMALL_X 228
#define ATLAS_CACTUS_SMALL_Y 2
#define ATLAS_CLOUD_X 86
#define ATLAS_CLOUD_Y 2
#define ATLAS_HORIZON_X 2
#define ATLAS_HORIZON_Y 54
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
	char digits[16];
	char highScore[16];
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
bool distanceMeterUpdate(int deltaTime, int _distance);
void distanceMeterDrawHighScore();
void distanceMeterSetHighScore(int _distance);
void distanceMeterReset();

void distanceMeterInit(int w) {
	distanceMeter.x = 0;
	distanceMeter.y = 5;
	distanceMeter.currentDistance = 0;
	distanceMeter.maxScore = 0;
	distanceMeter.achievement = false;
	distanceMeter.flashTimer = 0;
	distanceMeter.flashIterations = 0;
	distanceMeter.invertTrigger = false;
	distanceMeter.maxScoreUnits = DM_MAX_DISTANCE_UNITS;
	distanceMeterCalcXPos(w);
	for (int i = 0; i < distanceMeter.maxScoreUnits; i++) {
		distanceMeterDraw(i, 0, false);
	}
	distanceMeter.maxScore = (int)pow(10, distanceMeter.maxScoreUnits) - 1;
	distanceMeter.digits[0] = '\0';
	distanceMeter.highScore[0] = '\0';
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
	//printf("%d %d %d  %d %d    %d\n", digitPos, value, opt_highscore, dx, dy, DM_WIDTH * value + ATLAS_TEXT_SPRITE_X);
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

void distanceMeterDrawHighScore() {
	// TODO canvasCtx.globalAlpha = .8;
	for (int i = (int)strlen(distanceMeter.highScore) - 1; i >= 0; i--) {
		distanceMeterDraw(i, distanceMeter.highScore[i] > 12 ? distanceMeter.highScore[i] - '0' : distanceMeter.highScore[i], true);
	}
}

void distanceMeterSetHighScore(int _distance) {
	int distance = distanceMeterGetActualDistance(_distance);
	distanceMeter.highScore[0] = 10;
	distanceMeter.highScore[1] = 11;
	distanceMeter.highScore[2] = 12;
	intToStr(distance, distanceMeter.maxScoreUnits, distanceMeter.highScore + 3);
}

void distanceMeterReset() {
	distanceMeterUpdate(0, 0);
	distanceMeter.achievement = false;
}

int distanceMeterGetActualDistance(int distance) {
	return distance ? (int)round(distance * DM_COEFFICIENT) : 0;
}

bool distanceMeterUpdate(int deltaTime, int _distance) {
	bool paint = true;
	bool playSound = false;
	int distance = _distance;
	if (!distanceMeter.achievement) {
		distance = distanceMeterGetActualDistance(_distance);
		// check if score has gone beyond the initial digit count.
		if (distance > distanceMeter.maxScore && distanceMeter.maxScoreUnits == DM_MAX_DISTANCE_UNITS) {
			distanceMeter.maxScoreUnits++;
			distanceMeter.maxScore = distanceMeter.maxScore * 10 + 9;
		}
		// else {
			// this.distance was in original but i didsnt see any usage of this field
		// }

		if (distance > 0) {
			// Acheivement unlocked
			if (distance % DM_ACHIEVEMENT_DISTANCE == 0) {
				// Flash score and play sound
				distanceMeter.achievement = true;
				distanceMeter.flashTimer = 0;
				playSound = true;
			}
			// Create a string representation of the distance with leading 0.
			intToStr(distance, distanceMeter.maxScoreUnits, distanceMeter.digits);
		}
		else {
			intToStr(0, distanceMeter.maxScoreUnits, distanceMeter.digits);
		}
	}
	else {
		// Control flashing of the score on reaching acheivement.
		if (distanceMeter.flashIterations <= DM_FLASH_ITERATIONS) {
			distanceMeter.flashTimer += deltaTime;

			if (distanceMeter.flashTimer < DM_FLASH_DURATION) {
				paint = false;
			}
			else if (distanceMeter.flashTimer > DM_FLASH_DURATION * 2) {
				distanceMeter.flashTimer = 0;
				distanceMeter.flashIterations++;
			}
		}
		else {
			distanceMeter.achievement = false;
			distanceMeter.flashIterations = 0;
			distanceMeter.flashTimer = 0;
		}
	}

	// Draw the digits if not flashing
	if (paint) {
		for (int i = distanceMeter.maxScoreUnits - 1; i >= 0; i--) {
			distanceMeterDraw(i, (int)distanceMeter.digits[i] - '0', false);
		}
	}

	distanceMeterDrawHighScore();
	return playSound;
}

typedef struct {
	int width;
	int xPos;
	int yPos;
	bool remove;
	int cloudGap;
} Cloud;

void cloudInit();
void cloudDraw(const Cloud* cloud);
void cloudUpdate(Cloud* cloud, double speed);
bool cloudIsVisible(const Cloud* cloud);

void cloudInit(Cloud *cloud, int w) {
	cloud->width = w;
	cloud->xPos = w;
	cloud->yPos = 0;
	cloud->remove = false;
	cloud->cloudGap = getRandomNumber(CLOUD_MIN_GAP, CLOUD_MAX_GAP);
	
	cloud->yPos = getRandomNumber(CLOUD_MAX_SKY_LEVEL, CLOUD_MIN_SKY_LEVEL); // TODO why swapped
	cloudDraw(cloud);
}

void cloudDraw(const Cloud *cloud) {
	blitAtlasImage(ATLAS_CLOUD_X, ATLAS_CLOUD_Y, cloud->xPos, cloud->yPos, CLOUD_WIDTH, CLOUD_HEIGHT, false);
}

void cloudUpdate(Cloud *cloud, double speed) {
	if (!cloud->remove) {
		cloud->xPos -= (int)ceil(speed);
		cloudDraw(cloud);

		// Mark as removeable if no longer in the canvas.
		if (!cloudIsVisible(cloud)) {
			cloud->remove = true;
		}
	}
}

bool cloudIsVisible(const Cloud *cloud) {
	return cloud->xPos + CLOUD_WIDTH > 0;
}

#define HORIZON_LINE_WIDTH 600
#define HORIZON_LINE_HEIGHT 12
#define HORIZON_LINE_YPOS 127
struct {
	int width;
	int height;
	int sourceXPos[2];
	int xPos[2];
	int yPos;
	double bumpThreshold;
} horizonLine;

void horizonLineInit();
void horizonLineDraw();
int horizonLineGetRandomType();
void horizonLineUpdateXPos(int pos, int increment);
void horizonLineUpdate(int deltaTime, double speed);
void horizonLineReset();

void horizonLineInit() {
	horizonLine.width = HORIZON_LINE_WIDTH;
	horizonLine.height = HORIZON_LINE_HEIGHT;
	horizonLine.sourceXPos[0] = ATLAS_HORIZON_X;
	horizonLine.sourceXPos[1] = ATLAS_HORIZON_Y;
	horizonLine.bumpThreshold = 0.5;
	horizonLine.xPos[0] = 0;
	horizonLine.xPos[1] = horizonLine.width;
	horizonLine.yPos = HORIZON_LINE_YPOS;
	horizonLineDraw();
}

void horizonLineDraw() {
	blitAtlasImage(horizonLine.sourceXPos[0], ATLAS_HORIZON_Y, horizonLine.xPos[0], horizonLine.yPos, horizonLine.width, horizonLine.height, false);
	blitAtlasImage(horizonLine.sourceXPos[1], ATLAS_HORIZON_Y, horizonLine.xPos[1], horizonLine.yPos, horizonLine.width, horizonLine.height, false);
}

int horizonLineGetRandomType() {
	return (double)rand() / RAND_MAX > horizonLine.bumpThreshold ? DM_WIDTH : 0;
}

void horizonLineUpdateXPos(int pos, int increment) {
	int line1 = pos;
	int line2 = pos == 0 ? 1 : 0;

	horizonLine.xPos[line1] -= increment;
	horizonLine.xPos[line2] = horizonLine.xPos[line1] + DM_WIDTH;

	if (horizonLine.xPos[line1] <= -DM_WIDTH) {
		horizonLine.xPos[line1] += DM_WIDTH * 2;
		horizonLine.xPos[line2] = horizonLine.xPos[line1] - DM_WIDTH;
		horizonLine.sourceXPos[line1] = horizonLineGetRandomType() + ATLAS_HORIZON_X;
	}
}

void horizonLineUpdate(int deltaTime, double speed) {
	int increment = floor(speed * (FPS / 1000.0) * deltaTime);
	if (horizonLine.xPos[0] <= 0) {
		horizonLineUpdateXPos(0, increment);
	}
	else {
		horizonLineUpdateXPos(1, increment);
	}
	horizonLineDraw();
}

void horizonLineReset() {
	horizonLine.xPos[0] = 0;
	horizonLine.xPos[1] = DM_WIDTH;
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
	Ulist* obstacles;
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
	srand((unsigned int)time(NULL)); // Seed the random number generator

	//ulist_test();

	// The surface contained by the window
	//SDL_Surface* screenSurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	//Create window
	SDL_Window *window = SDL_CreateWindow("C Rex Runner", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1300, 480, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(renderer, 0xF7, 0xF7, 0xF7, 0xFF);
	SDL_RenderClear(renderer);

	runnerInit();

	//// just test
	//horizonLineInit();
	// Cloud c;
	// for (int i = 0; i < 15; i++) {
	// 	cloudInit(&c, runner.width);
	// 	c.xPos = i * 60;
	// 	cloudDraw(&c);
	// }
	// 
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

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
