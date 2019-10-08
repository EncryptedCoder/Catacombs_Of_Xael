#pragma once
#include "Player.h"
#include "Enemy.h"

class Player;

class Mummy :
	public Enemy
{
public: //functions
	Mummy(SDL_Renderer& renderer, std::string& spriteSheet, std::string& mName, char &symbol, float &level, float &health, int &endurance, int &money, float& yPos, float& xPos, int &zPos, int &bonusHeal);
	virtual ~Mummy();

	bool move(Player *player, Zone *zone, GameManager *GM) ;
	void render(SDL_Renderer& renderer);
	void update(Zone* zone);

	SDL_Rect destRect;
private:
	SDL_Texture* mummyTexture;
	SDL_Rect srcRect;
protected:
};