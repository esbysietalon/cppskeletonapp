// GraphicsBase.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Skeleton.h"








Skeleton::Skeleton(int width, int height, char* t) {
	screenWidth = width;
	screenHeight = height;
	title = t;
	init();
	addRunFunc(std::bind(&Skeleton::render, this));
}
Skeleton::~Skeleton() {
	std::cout << "Shutting down.." << std::endl;
	funcs.clear();
	std::cout << "Run functions cleared.." << std::endl;
	close();
	std::cout << "SDL shut down.." << std::endl;
	std::cout << "Shutdown successful.." << std::endl;
} 

bool Skeleton::init() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_ERROR: %s\n", SDL_GetError());
		success = false;
	}
	else {
		gWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created! SDL_ERROR: %s\n", SDL_GetError());
			success = false;
		}
		else {
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
		renderer = SDL_CreateRenderer(gWindow, -1, 0);
	}

	return success;
}

int Skeleton::loadMedia(char* filepath) {
	SDL_Surface* media = NULL;

	media = SDL_LoadBMP(filepath);
	
	if (media == NULL) {
		printf("Unable to load image %s! SDL ERROR:%s\n", "filepath", SDL_GetError());
	}
	else {
		return addSurface(media);
	}
	
	//return media;
	return -1;
}

void Skeleton::setFrameCap(int num)
{
	fps = num;
}

void Skeleton::addRunFunc(std::function<void()> func)
{
	funcs.emplace_back(func);
}

void Skeleton::run() {
	int startTime = SDL_GetTicks();
	int cap = (int)(1000.0 / fps);
	for(int i = 0; i < funcs.size(); i++){
		if(running)
			funcs.at(i)();
	}
	/*if(funcs.size() > 0)
		for (std::vector<std::function<void()>>::iterator it = funcs.begin(); it != funcs.end(); it++){
			(*it)();
		}*/
	int nowTime = SDL_GetTicks();
	int delta = nowTime - startTime;
	if (delta < cap) {
		SDL_Delay(cap - delta);
	}
}

/*void Skeleton::capFrames(int fps, std::function<void()>* funcs, int flen) {
	int startTime = SDL_GetTicks();
	int cap = (int)(1000.0 / fps);
	for (int i = 0; i < flen; i++) {
		funcs[i]();
	}
	int nowTime = SDL_GetTicks();
	int delta = nowTime - startTime;
	if (delta < cap) {
		SDL_Delay(cap - delta);
	}
}*/

int Skeleton::createTexture(int* pixels, int w, int h) {
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, w, h);
	SDL_UpdateTexture(texture, NULL, pixels, w * sizeof(Uint32));
	Skin* skin = new Skin;
	skin->texture = texture;
	skin->w = w;
	skin->h = h;
	return addSkin(skin);
}

int Skeleton::createSprite(int textureindex, int x, int y)
{
	Sprite* sprite = new Sprite;

	sprite->skin = skinMap[textureindex];
	sprite->x = x;
	sprite->y = y;
	return addSprite(sprite);
}

bool Skeleton::removeSprite(int spriteindex)
{
	if (spriteMap.count(spriteindex) > 0) {
		spriteMap.erase(spriteindex);
		spriteRegistry.erase(spriteindex);
		return true;
	}
	return false;
}

int Skeleton::addSurface(SDL_Surface* surface) {
	int newId = surfaceRegistered;
	while (surfaceRegistry.count(newId = surfaceRegistered) > 0)
		surfaceRegistered++;
	surfaceMap[newId] = surface;
	surfaceRegistry.emplace(newId);
	return newId;
}
int Skeleton::addSkin(Skin* skin) {
	int newId = skinRegistered;
	while (skinRegistry.count(newId = skinRegistered) > 0)
		skinRegistered++; 
	skinMap[newId] = skin;
	skinRegistry.emplace(newId);
	return newId;
}
int Skeleton::addSprite(Sprite* sprite) {
	int newId;
	while (spriteRegistry.count(newId = spriteRegistered) > 0)
		spriteRegistered++;
	spriteMap[newId] = sprite;
	spriteRegistry.emplace(newId);
	return newId;
}

int Skeleton::addFunction(ContextFunction * func)
{
	int newId;
	while (funcRegistry.count(newId = funcRegistered) > 0)
		funcRegistered++;
	funcMap[newId] = func;
	funcRegistry.emplace(newId);
	return newId;
}



void Skeleton::close() {
	std::cout << "Destroying Textures.." << std::endl;
	for (std::unordered_map<int, Skin*>::iterator it = skinMap.begin(); it != skinMap.end(); it++) {
		SDL_DestroyTexture((*it).second->texture);
	}
	std::cout << "Textures destroyed.." << std::endl;
	std::cout << "Freeing Surfaces.." << std::endl;
	for (std::unordered_map<int, SDL_Surface*>::iterator it = surfaceMap.begin(); it != surfaceMap.end(); it++) {
		SDL_FreeSurface((*it).second);
	}
	std::cout << "Surfaces freed.." << std::endl;
	std::cout << "Destroying Renderer.." << std::endl;
	SDL_DestroyRenderer(renderer);
	std::cout << "Destroying Window.." << std::endl;
	SDL_DestroyWindow(gWindow);
	std::cout << "Quitting SDL.." << std::endl;
	SDL_Quit();
}

Sprite * Skeleton::getSprite(int spriteIndex)
{
	Sprite* sprite = NULL;
	if (spriteMap.count(spriteIndex) > 0)
		sprite = spriteMap[spriteIndex];
	return sprite;
}

void Skeleton::registerFunction(std::function<void()> func, SDL_EventType type, SDL_Scancode key)
{
	ContextFunction* newfunc = new ContextFunction;
	newfunc->func = func;
	newfunc->type = type;
	newfunc->key = key;
	
	addFunction(newfunc);
}

int Skeleton::listen() {
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			running = false;
			funcs.clear();
			return 0;
		}

		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		for (std::unordered_map<int, ContextFunction*>::iterator it = funcMap.begin(); it != funcMap.end(); it++) {
			ContextFunction* f = (*it).second;
			if (f->type == e.type) {
				if (f->type == SDL_KEYDOWN) {
					if (currentKeyStates[f->key]) {
						f->func();
					}
				}
				else if (f->type == SDL_KEYUP) {
					if (!currentKeyStates[f->key]) {
						f->func();
					}
				}
				else {
					f->func();
				}
			}
		}
	}
	return 1;
}

void Skeleton::render() {
	SDL_RenderClear(renderer);
	for (std::unordered_map<int, Sprite*>::iterator it = spriteMap.begin(); it != spriteMap.end(); it++) {
		SDL_Rect dstrect;
		dstrect.x = (*it).second->x;
		dstrect.y = (*it).second->y;
		dstrect.w = (*it).second->skin->w;
		dstrect.h = (*it).second->skin->h;
		//std::cout << dstrect.x << std::endl;
		//std::cout << dstrect.y << std::endl;
		//std::cout << dstrect.w << std::endl;
		//std::cout << dstrect.h << std::endl;
		
		//SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadMedia("Resources/hello_world.bmp"));
		//std::cout << renderer << std::endl;
		SDL_RenderCopy(renderer, (*it).second->skin->texture, NULL, &dstrect);
	}
	SDL_RenderPresent(renderer);
	//SDL_UpdateWindowSurface(gWindow);
}

