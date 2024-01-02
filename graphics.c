#include "graphics.h"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* graphicsSpriteAtlas;

void graphicsInit() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	window = SDL_CreateWindow("C Rex Runner", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1300, 480, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	graphicsSpriteAtlas = IMG_LoadTexture(renderer, "assets/sprites100.png");	

}

void graphicsBlitAtlasImage(int atlasX, int atlasY, int destX, int destY, int w, int h, bool center) {
	SDL_Rect dest = { destX, destY, w, h }, src = { atlasX, atlasY, w, h };
	if (center) {
		dest.x -= (dest.w / 2);
		dest.y -= (dest.h / 2);
	}
	SDL_RenderCopy(renderer, graphicsSpriteAtlas, &src, &dest);
}