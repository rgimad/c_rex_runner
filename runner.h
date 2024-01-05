#pragma once

#include <stdbool.h>
#include "config.h"
#include "ulist.h"
#include "graphics.h"
#include "distance_meter.h"

#define RUNNER_DEFAULT_HEIGHT 150

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

typedef struct {
	int width;
	int height;
	// trex
	// distancemeter
	int distanceRan;
	int highestScore;
	int time;
	int runningTime;
	double msPerFrame;
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
	// images
	// imagesLoaded
} Runner;

extern Runner runner;

void runnerInit();
void runnerAdjustDimensions();
void runnerLoadImages();
