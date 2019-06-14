// GraphicsBase.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GraphicsBase.h"

#include <windows.h>    // include the basic windows header file
#include <SDL.h>
#include <iostream>

#define SCREEN_W 800
#define SCREEN_H 600

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gHelloWorld = NULL;

bool init() {
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

bool loadMedia() {
	bool success = true;

	gHelloWorld = SDL_LoadBMP("Resources/hello_world.bmp");
	if (gHelloWorld == NULL) {
		printf("Unable to load image %s! SDL ERROR:%s\n", "Resources/hello_world.bmp", SDL_GetError());
		success = false;
	}
	
	return success;
}

void close() {
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();
}

int main(int argc, char** args) {

	bool quit = false;
	SDL_Event e;

	if (!init()) {
		printf("Failed to initialize SDL!\n");
	}
	else {
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		}
		else {
			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}
				}

				SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
				SDL_UpdateWindowSurface(gWindow);
			}
			
			
		}
	}

	
	
	close();
	return 0;
}