#pragma once
#include "Zone.h"
#include "GameManager.h"
#include "GameManager.h"
#include "TextureManager.h"

#include <string>
#include "../SDL2/include/SDL.h"
#include "../SDL2/include/SDL_render.h"



class Window
{
public:
	Window(int &width, int &height, Zone *maps);
	~Window();

	SDL_Window* sdl_pWindow = nullptr;

	bool running() { return isRunning; }
	void handleEvents();

	void render(SDL_Renderer* renderer, int &zPos);
	void init(std::string& mfile);
	void clean();
	void setMapTextures(SDL_Renderer& r);

	void MapInfo(std::string& mfile);

private:

	int _mapTilesWide;
	int _mapTilesHeight;
	int _tileWidth;
	int _tileHeight;
	int _windowWidth;
	int _windowHeight;

	std::string text_mapWide;
	std::string text_mapHeight;
	std::string text_tileWidth;
	std::string text_tileHeight;

	std::string title;
	

	bool isRunning;


};

