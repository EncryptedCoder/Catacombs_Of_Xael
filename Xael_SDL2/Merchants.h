#pragma once
#include "Entity.h"
#include "Enemy.h"
#include "Player.h"

class Player;

class FloorObjects : public Enemy
{
public:
	FloorObjects(SDL_Renderer& renderer, std::string& spriteSheet, std::string &oName, char& symbol, float& level, float& health, int& endurance, int& money, int& pos, int& posZ, int& bonusHeal);
	virtual ~FloorObjects();

	int move(Player *player, Zone *zone, GameManager *GM);

	void render(SDL_Renderer& renderer);
	void update(Zone* zone);

private:

	SDL_Texture* chestTexture;
	SDL_Rect srcRect, destRect;

	int row_y = 0;
	int col_x = 0;

protected:
	


};

