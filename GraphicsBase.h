#pragma once

#include "resource.h"
#include <windows.h>    // include the basic windows header file
#include <SDL.h>
#include <iostream>
struct Sprite {
	SDL_Texture* surface;
	int x, y;
};

class Graphics {
public:
	Graphics();
	~Graphics();
	void loadMedia(char* filepath);
	
	void createTexture();
	
	bool listenExit();
	void render();
private:
	bool init();
	void close();
	int addSurface(SDL_Surface* surface);

	int numSurfaces = 0;
	int curSurfaceSize = 1;
	int numTextures = 0;
	int curTextureSize = 1;
	int numSprites = 0;
	int curSpriteSize = 1;

	Sprite** sprites = NULL;

	SDL_Surface** surfaces = NULL;
	SDL_Texture** textures = NULL;

	SDL_Window* gWindow = NULL;
	SDL_Surface* gScreenSurface = NULL;
	SDL_Renderer*  renderer = NULL;
};