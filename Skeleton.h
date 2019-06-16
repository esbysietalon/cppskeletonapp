#pragma once

#include "resource.h"
#include <windows.h>    // include the basic windows header file
#include <SDL.h>
#include <iostream>
#include <functional>

struct ContextFunction {
	std::function<void()> func;
	SDL_EventType type;
	SDL_Scancode key;
};

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
	
	int createTexture(int* pixels, int w, int h);
	int createSprite(int textureindex, int x, int y);
	Sprite* getSprite(int spriteIndex);

	void registerFunction(void(*func)(), SDL_EventType type, SDL_Scancode key = SDL_SCANCODE_LANG1);
	int listen();
	void render();
	void capFrames(int fps, std::function<void()>* funcs, int flen);
private:
	bool init();

	int screenWidth = 300;
	int screenHeight = 300;

	void close();

	

	int addSurface(SDL_Surface* surface);
	int addSprite(Sprite* sprite);
	int addSkin(Skin* skin);
	int addFunction(ContextFunction* func);
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

	ContextFunction** registeredFuncs = NULL;
	int numFuncs = 0;
	int registrySize = 1;
};