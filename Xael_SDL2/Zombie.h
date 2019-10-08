#pragma once
#include "Player.h"
#include "Enemy.h"

class Player;

class Zombie :
	public Enemy
{
public: //functions
	Zombie(SDL_Renderer& renderer, std::string& spriteSheet, std::string& zName, char &symbol, float &level, float &health, int &endurance, int &money, float& yPos, float& xPos, int &zPos, int &bonusHeal);
	virtual ~Zombie();

	bool move(Player *player, Zone *zone, GameManager *GM) ;

	void render(SDL_Renderer& renderer);
	void update(Zone* zone);

	SDL_Rect destRect;
private:
	SDL_Texture* zombieTexture;
	SDL_Rect srcRect;

protected:
};