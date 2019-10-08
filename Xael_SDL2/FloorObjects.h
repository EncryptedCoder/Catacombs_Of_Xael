#pragma once
#include "Entity.h"
#include "Enemy.h"
#include "Player.h"

class Player;

class FloorObjects : public Enemy
{
public:
	FloorObjects(SDL_Renderer& renderer, std::string& spriteSheet, std::string &oName, char& symbol, float& level, float& health, int& endurance, int& money, float& yPos, float& xPos, int& zPos, int& bonusHeal);
	virtual ~FloorObjects();

	bool move(Player *player, Zone *zone, GameManager *GM) ;

	void render(SDL_Renderer& renderer);
	void update(Zone* zone);

	SDL_Rect destRect;
private:

	SDL_Texture* chestTexture;
	SDL_Rect srcRect[2];
	SDL_Rect curRectText;

protected:
	


};

