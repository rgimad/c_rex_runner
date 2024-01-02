#pragma once

#include <stdbool.h>
#include <math.h>
#include "graphics.h"

#define DM_MAX_DISTANCE_UNITS 5
#define DM_ACHIEVEMENT_DISTANCE 100
#define DM_COEFFICIENT 0.025
#define DM_FLASH_DURATION 1000/4
#define DM_FLASH_ITERATIONS 3
#define DM_WIDTH 10
#define DM_HEIGHT 13
#define DM_DEST_WIDTH 11

typedef struct {
	int x;
	int y;
	int currentDistance;
	int maxScore;
	char digits[16];
	char highScore[16];
	bool achievement;
	int flashTimer;
	int flashIterations;
	bool invertTrigger;
	int maxScoreUnits;
} DistanceMeter;

extern DistanceMeter distanceMeter;

void distanceMeterInit(int w);
void distanceMeterCalcXPos(int w);
void distanceMeterDraw(int digitPos, int value, bool opt_highscore);
int distanceMeterGetActualDistance(int distance);
bool distanceMeterUpdate(int deltaTime, int _distance);
void distanceMeterDrawHighScore();
void distanceMeterSetHighScore(int _distance);
void distanceMeterReset();

