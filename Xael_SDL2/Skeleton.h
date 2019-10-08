#pragma once
#include "Player.h"
#include "Enemy.h"

class Player;

class Skeleton : public Enemy
{
public://functions
	Skeleton(SDL_Renderer& renderer, std::string& spriteSheet, std::string& sName, char &symbol, float &level, float &health, int &endurance, int &money, float& yPos, float& xPos, int &zPos, int &bonusHeal);
	virtual ~Skeleton();
	bool move(Player *player, Zone *zone, GameManager *GM) ;
	void render(SDL_Renderer& renderer);
	void update(Zone* zone);

	SDL_Rect destRect;

private:
	SDL_Texture* skeletonTexture;
	SDL_Rect srcRect;

protected:
	
};