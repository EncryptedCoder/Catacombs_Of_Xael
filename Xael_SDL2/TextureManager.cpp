#include "TextureManager.h"

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}


SDL_Texture* TextureManager::LoadTexture(SDL_Renderer& renderer, std::string &texture)
{
	//creating surface to load an img too.
	SDL_Surface* tempSurface = IMG_Load(texture.c_str());
	//placing that image from surface to a texture
	SDL_Texture* imgTexture = SDL_CreateTextureFromSurface(&renderer, tempSurface);
	//now clearing the surface because image is saved in the texture.
	SDL_FreeSurface(tempSurface);

	return imgTexture;
}


void TextureManager::Draw(SDL_Renderer* renderer, SDL_Texture* tex, SDL_Rect src, SDL_Rect dest)
{
	SDL_RenderCopy(renderer, tex, &src, &dest);
}