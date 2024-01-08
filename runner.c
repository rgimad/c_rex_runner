#include "runner.h"

Runner runner;

void runnerInit() {
	runner.distanceRan = 0;
	runner.highestScore = 0;
	runner.time = 0;
	runner.msPerFrame = 1000 / FPS;
	runner.currentSpeed = RUNNER_SPEED;
	//runner.obstacles = ulist_create();
	runner.activated = false;
	runner.playing = false;
	runner.crashed = false;
	runner.paused = false;
	runner.inverted = false;
	runner.playingIntro = false;
	//runner.isRunning = true;
	runner.invertTimer = 0;
	runner.playCount = 0;
	// TODO sound
	// runnerLoadImages();
	runnerAdjustDimensions();
	// setSpeed
	graphicsFillBackground(0xF7, 0xF7, 0xF7);

	gameOverPanelInit(runner.width, runner.height);
	
	horizonInit(runner.width, RUNNER_GAP_COEFFICIENT);
	distanceMeterInit(runner.width);
	trexInit();

	// gameOverPanelDraw(); // just test

	/*this.startListening();
	this.update();
	window.addEventListener(Runner.events.RESIZE,
		this.debounceResize.bind(this));*/
}

void runnerAdjustDimensions() {
	runner.width = DEFAULT_WIDTH;
	runner.height = RUNNER_DEFAULT_HEIGHT;
	// distance meter ...
}

void runnerOnKeyDown(int key) {
	if (!runner.crashed && (key == RUNNER_KEYCODE_JUMP_1 || key == RUNNER_KEYCODE_JUMP_2)) {
		if (!runner.playing) {
			// this.loadSounds(); // TODO
			runner.playing = true;
			runnerUpdate();
		}
		//  Play sound effect and jump on starting the game for the first time.
		if (!trex.jumping && !trex.ducking) {
			// this.playSound(this.soundFx.BUTTON_PRESS); // TODO
			trexStartJump(runner.currentSpeed);
		}
	}
	if (runner.playing && !runner.crashed && key == RUNNER_KEYCODE_DUCK) {
		if (trex.jumping) {
			// Speed drop, activated only when jump key is not pressed.
			trexSetSpeedDrop();
		}
		else {
			// Duck
			trexSetDuck(true);
		}
	}
}

void runnerOnKeyUp(int key) {
	if (/*runner.isRunning &&*/ (key == RUNNER_KEYCODE_JUMP_1 || key == RUNNER_KEYCODE_JUMP_2)) {
		trexEndJump();
	}
	else if (key == RUNNER_KEYCODE_DUCK) {
		trex.speedDrop = false;
		trexSetDuck(false);
	}
	else if (runner.crashed) {
		// Check that enough time has elapsed before allowing jump key to restart.
		int deltaTime = getTimeStamp() - runner.time;
		if (key == RUNNER_KEYCODE_RESTART || (deltaTime >= RUNNER_GAMEOVER_CLEAR_TIME && (key == RUNNER_KEYCODE_JUMP_1 || key == RUNNER_KEYCODE_JUMP_2))) {
			runnerRestart();
		}
	}
	else if (runner.paused && (key == RUNNER_KEYCODE_JUMP_1 || key == RUNNER_KEYCODE_JUMP_2)) {
		trexReset();
		runnerPlay();
	}
}


void runnerUpdate() {
	int now = getTimeStamp();
	int deltaTime = now - (runner.time ? runner.time : 0);
	runner.time = now;
	if (runner.playing) {
		graphicsFillBackground(0xF7, 0xF7, 0xF7);

		if (trex.jumping) {
			trexUpdateJump(deltaTime);
		}

		runner.runningTime += deltaTime;
		bool hasObstacles = runner.runningTime > RUNNER_CLEAR_TIME;

		// First jump triggers the intro.
		if (trex.jumpCount == 1 && !runner.playingIntro) {
			runnerPlayIntro();
		}

		// The horizon doesn't move until the intro is over.
		if (runner.playingIntro) {
			horizonUpdate(0, runner.currentSpeed, hasObstacles, false);
		}
		else {
			deltaTime = !runner.activated ? 0 : deltaTime;
			horizonUpdate(deltaTime, runner.currentSpeed, hasObstacles, runner.inverted);
		}

		// Check for collisions.
		bool collision = hasObstacles && runnerCheckForCollision(horizon.obstacles->head->data);

		if (!collision) {
			runner.distanceRan += runner.currentSpeed * deltaTime / runner.msPerFrame;

			if (runner.currentSpeed < RUNNER_MAX_SPEED) {
				runner.currentSpeed += RUNNER_ACCELERATION;
			}
		}
		else {
			runnerGameOver();
		}

		bool playAchievementSound = distanceMeterUpdate(deltaTime, (int)ceil(runner.distanceRan));

		if (playAchievementSound) {
			//this.playSound(this.soundFx.SCORE); // TODO
		}

		/*// Night mode.
		if (this.invertTimer > this.config.INVERT_FADE_DURATION) {
			this.invertTimer = 0;
			this.invertTrigger = false;
			this.invert();
		}
		else if (this.invertTimer) {
			this.invertTimer += deltaTime;
		}
		else {
			var actualDistance =
				this.distanceMeter.getActualDistance(Math.ceil(this.distanceRan));

			if (actualDistance > 0) {
				this.invertTrigger = !(actualDistance %
					this.config.INVERT_DISTANCE);

				if (this.invertTrigger&& this.invertTimer == = 0) {
					this.invertTimer += deltaTime;
					this.invert();
				}
			}
		}*/
	}

	if (runner.playing || (!runner.activated && trex.blinkCount < RUNNER_MAX_BLINK_COUNT)) {
		trexUpdate(deltaTime, -1);
		// this.scheduleNextUpdate();
	}
}

void runnerGameOver() {
	// this.playSound(this.soundFx.HIT); // TODO
	runnerStop();
	runner.crashed = true;
	distanceMeter.achievement = false;
	trexUpdate(100, TREX_STATUS_CRASHED);

	// Game over panel
	gameOverPanelDraw();
	// Update the high score
	if (runner.distanceRan > runner.highestScore) {
		runner.highestScore = (int)ceil(runner.distanceRan);
		distanceMeterSetHighScore(runner.highestScore);
	}
	// Reset the time clock
	runner.time = getTimeStamp();
}

void runnerStop() {
	runner.playing = false;
	runner.paused = true;
	//runner.isRunning = false;
}

void runnerPlay() {
	if (!runner.crashed) {
		runner.playing = true;
		runner.paused = false;
		trexUpdate(0, TREX_STATUS_RUNNING);
		runner.time = getTimeStamp();
		runnerUpdate();
	}
}

void runnerRestart() {
	// if (!this.raqId) {
	runner.playCount++;
	runner.runningTime = 0;
	runner.playing = true;
	runner.crashed = false;
	runner.distanceRan = 0;
	//runnerSetSpeed(RUNNER_SPEED); // need for mobile
	runner.time = getTimeStamp();
	//this.clearCanvas();
	graphicsFillBackground(0xF7, 0xF7, 0xF7);
	distanceMeterReset(runner.highestScore);
	horizonReset();
	trexReset();
	//this.playSound(this.soundFx.BUTTON_PRESS);
	//this.invert(true);
	runnerUpdate();
	// }
}

void runnerPlayIntro() {
	if (!runner.activated && !runner.crashed) {
		runner.playingIntro = true;
		trex.playingIntro = true;
		// ...

		runnerStartGame();

		runner.playing = true;
		runner.activated = true;
	}
	else if (runner.crashed) {
		runnerRestart();
	}
}

void runnerStartGame() {
	runner.runningTime = 0;
	runner.playingIntro = false;
	trex.playingIntro = false;
	runner.playCount++;
}


