#pragma once
#include "Player.h"
#include "Enemy.h"

class Player;

class Boss :
	public Enemy
{
public: //
	Boss(SDL_Renderer& renderer, std::string& spriteSheet, std::string &bName, char &symbol, float &level, float &health, int &endurance, int &money, float& yPos, float& xPos, int &zPos, int &bonusHeal);
	virtual ~Boss();

	bool move(Player* player, Zone* zone, GameManager* GM);

	void render(SDL_Renderer& renderer);
	void update(Zone* zone);

	SDL_Rect destRect;
private://

	

	SDL_Texture* bossTexture;
	SDL_Rect srcRect;
};