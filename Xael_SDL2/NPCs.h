#pragma once
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"

class Player;


class NPCs : public Entity
{
public:
	NPCs(SDL_Renderer& renderer, std::string& spriteSheet, std::string &oName, char& symbol, float& level, float& health, int& endurance, int& money, float& yPos, float& xPos, int& zPos);
	virtual ~NPCs();

	void npcDetails(Player *player, Zone *zone, GameManager *GM);

	void render(SDL_Renderer& renderer);
	void update(Zone* zone);
	void updatePortal(Zone* zone);
	bool move(Player* player, Zone* zone, GameManager* GM) ;
	SDL_Rect destRect;
private:

	SDL_Texture *npcTextures;
	SDL_Rect srcRect[4];
	SDL_Rect currentFrame;
	int animFrame = 0;
	int slowDowFrame = 0;

	float posX;
	float posY;
	int zPos;

protected:
	

};

