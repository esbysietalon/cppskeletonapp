#pragma once

#include "resource.h"
#include <windows.h>    // include the basic windows header file
#include <SDL.h>
#include <iostream>

struct Skin {
	SDL_Texture* texture;
	int w, h;
};
struct Sprite {
	Skin* skin;
	int x, y;
};
class Skeleton {
public:
	Skeleton(int w, int h);
	~Skeleton();
	int loadMedia(char* filepath);
	
	int createTexture(Uint32* pixels, int w, int h);
	int createSprite(int textureindex, int x, int y);

	bool listenExit();
	void render();
private:
	bool init();

	int screenWidth = 300;
	int screenHeight = 300;

	void close();

	int addSurface(SDL_Surface* surface);
	int addSprite(Sprite* sprite);
	int addSkin(Skin* skin);
	int numSurfaces = 0;
	int curSurfaceSize = 1;
	int numSkins = 0;
	int curSkinSize = 1;
	int numSprites = 0;
	int curSpriteSize = 1;

	Sprite** sprites = NULL;

	SDL_Surface** surfaces = NULL;
	Skin** skins = NULL;

	SDL_Window* gWindow = NULL;
	SDL_Surface* gScreenSurface = NULL;
	SDL_Renderer*  renderer = NULL;
};