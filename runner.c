#include "runner.h"

Runner runner;

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
	// runnerLoadImages();
	runnerAdjustDimensions();
	// fill canvas 
	// horizon create
	distanceMeterInit(runner.width);
}

void runnerAdjustDimensions() {
	runner.width = DEFAULT_WIDTH;
	// distance meter ...
}