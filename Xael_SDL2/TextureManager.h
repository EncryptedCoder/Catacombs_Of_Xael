#pragma once
#include "Window.h"

#include "../SDL2_image/include/SDL_image.h"
#include "../SDL2/include/SDL.h"
#include "../SDL2/include/SDL_render.h"

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	static SDL_Texture* LoadTexture(SDL_Renderer& renderer, std::string &fileName);
	static void Draw(SDL_Renderer* renderer, SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
};

