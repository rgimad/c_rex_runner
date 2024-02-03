#include "graphics.h"
#include "sprites.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Texture* graphicsSpriteAtlas;

static SDL_Texture* LoadTextureFromMemory(const void* mem, int size) {
	int width, height, channels, format;
	int inforet = stbi_info_from_memory(mem, size, &width, &height, &format);
	// no alpha => use RGB, else use RGBA
	int bppToUse = (format == STBI_grey || format == STBI_rgb) ? STBI_rgb : STBI_rgb_alpha;

	stbi_uc *imgdata = stbi_load_from_memory(mem, size, &width, &height, &format, bppToUse);
	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(imgdata, width, height, bppToUse * 8, bppToUse * width, (bppToUse == STBI_rgb) ? SDL_PIXELFORMAT_RGB24 : SDL_PIXELFORMAT_RGBA32);
	if (!surface) {
		printf("Error: %s\n", SDL_GetError());
		exit(-1);
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

void graphicsInit() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	window = SDL_CreateWindow("C Rex Runner", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_WIDTH, 200, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1"); // prevent jerking
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//graphicsSpriteAtlas = IMG_LoadTexture(renderer, "assets/sprites100.png");	
	graphicsSpriteAtlas = LoadTextureFromMemory(sprites100, sizeof(sprites100));
}

void graphicsBlitAtlasImage(int atlasX, int atlasY, int destX, int destY, int w, int h, bool center) {
	SDL_Rect dest = { destX, destY, w, h }, src = { atlasX, atlasY, w, h };
	if (center) {
		dest.x -= (dest.w / 2);
		dest.y -= (dest.h / 2);
	}
	SDL_RenderCopy(renderer, graphicsSpriteAtlas, &src, &dest);
}

void graphicsFillBackground(unsigned r, unsigned g, unsigned b) {
	SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
	SDL_RenderClear(renderer);
}

void graphicsRender() {
	SDL_RenderPresent(renderer);
}

void graphicsDelay(int ms) {
	SDL_Delay(ms);
}


void graphicsDestroy() {
	//Destroy window
	SDL_DestroyWindow(window);
	//Quit SDL subsystems
	SDL_Quit();
}
