// GraphicsBase.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GraphicsBase.h"



#define SCREEN_W 800
#define SCREEN_H 600





Graphics::Graphics() {
	init();
}
Graphics::~Graphics() {
	close();
}

bool Graphics::init() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_ERROR: %s\n", SDL_GetError());
		success = false;
	}
	else {
		gWindow = SDL_CreateWindow("Graphics Base (SDL)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created! SDL_ERROR: %s\n", SDL_GetError());
			success = false;
		}
		else {
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}

void Graphics::loadMedia(char* filepath) {
	SDL_Surface* media = NULL;

	media = SDL_LoadBMP(filepath);
	
	if (media == NULL) {
		printf("Unable to load image %s! SDL ERROR:%s\n", "filepath", SDL_GetError());
	}
	else {
		addSurface(media);
	}
	
}

void Graphics::createTexture() {
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 640, 480);
}

int Graphics::addSurface(SDL_Surface* surface) {
	if (surface == NULL)
		return -1;
	numSurfaces++;
	if (numSurfaces >= curSurfaceSize) {
		surfaces = (SDL_Surface**)realloc(surfaces, sizeof(SDL_Surface*) * numSurfaces * 2);
		curSurfaceSize = numSurfaces * 2;
	}
	surfaces[numSurfaces - 1] = surface;
	return numSurfaces - 1;
}

void Graphics::close() {
	for (int i = 0; i < numSurfaces; i++) {
		SDL_FreeSurface(surfaces[i]);
	}
	surfaces = NULL;
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
}

bool Graphics::listenExit() {
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			return true;
		}
	}
	return false;
}

void Graphics::render() {
	for(int i = 0; i < numSurfaces; i++)
		SDL_BlitSurface(surfaces[i], NULL, gScreenSurface, NULL);
	SDL_UpdateWindowSurface(gWindow);
}

