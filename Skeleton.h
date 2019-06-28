#pragma once

#include <windows.h>    // include the basic windows header file
#include <SDL.h>
#include <functional>
#include <vector>
#include <unordered_map>
#include <set>
#include <iostream>

struct ContextFunction {
	std::function<void()> func;
	SDL_EventType type;
	SDL_Scancode key;
};
struct intpair {
	int x, y;
};
struct Skin {
	SDL_Texture* texture;
	int w, h;
};
struct Sprite {
	Skin* skin;
	int x, y;
	intpair getCenter() {
		intpair point;
		point.x = x + skin->w / 2;
		point.y = y + skin->h / 2;
		return point;
	}
	intpair getEdge() {
		intpair point;
		point.x = x + skin->w;
		point.y = y + skin->h;
		return point;
	}
};
class Skeleton {
public:
	Skeleton(int w, int h);
	~Skeleton();
	int loadMedia(char* filepath);
	
	int createTexture(int* pixels, int w, int h);
	int createSprite(int textureindex, int x, int y);
	bool removeSprite(int spriteindex);
	Sprite* getSprite(int spriteIndex);

	void registerFunction(void(*func)(), SDL_EventType type, SDL_Scancode key = SDL_SCANCODE_LANG1);
	int listen();
	void render();
	void run();
	void setFrameCap(int fps);
	void addRunFunc(std::function<void()> func);
	//void capFrames(int fps, std::function<void()>* funcs, int flen);
private:
	bool init();

	bool running = true;

	int screenWidth = 300;
	int screenHeight = 300;
	int fps = 60;

	void close();

	int addSurface(SDL_Surface* surface);
	int addSprite(Sprite* sprite);
	int addSkin(Skin* skin);
	int addFunction(ContextFunction* func);
	
	std::vector<std::function<void()>> funcs;
	
	std::unordered_map<int, Sprite*> spriteMap;
	std::unordered_map<int, SDL_Surface*> surfaceMap;
	std::unordered_map<int, Skin*> skinMap;
	std::unordered_map<int, ContextFunction*> funcMap;

	std::set<int> spriteRegistry;
	std::set<int> surfaceRegistry;
	std::set<int> skinRegistry;
	std::set<int> funcRegistry;

	int spriteRegistered = 0;
	int surfaceRegistered = 0;
	int skinRegistered = 0;
	int funcRegistered = 0;

	SDL_Window* gWindow = NULL;
	SDL_Surface* gScreenSurface = NULL;
	SDL_Renderer*  renderer = NULL;
};