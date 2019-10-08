#include "FloorObjects.h"

FloorObjects::FloorObjects(SDL_Renderer& renderer, std::string& spriteSheet, std::string& oName, char& symbol, float& level, float& health, int& endurance, int& money, float& yPos, float& xPos, int& zPos, int& bonusHeal) :
					Enemy(renderer, spriteSheet, symbol, oName, level, health, endurance, money, yPos, xPos, zPos, bonusHeal)
{
	chestTexture = TextureManager::LoadTexture(renderer, spriteSheet);

	//width and height of each spot in a sprite.
	srcRect[0].h = 32;
	srcRect[0].w = 32;
	//start of sprite animation
	srcRect[0].x = 0;
	srcRect[0].y = 0;

	//width and height
	srcRect[1].h = 32;
	srcRect[1].w = 32;
	//2nd sprite animation
	srcRect[1].x = 32;
	srcRect[1].y = 0;
}


FloorObjects::~FloorObjects()
{

}

bool FloorObjects::move(Player* player, Zone* zone, GameManager* GM)
{
	int tmpZ = this->getZpos();
	int hitChance = 0;  //get a random number
	bool doesHit = false;
	static float moveY = 0.0f;
	static float moveX = 0.0f;
	//checking to see if the enemy is next to m_pPlayer.
   //will eventually add range in place of 1...
	if (player->getZpos() == this->getZpos() && GM->checkCollision(player->destRect, this->destRect))
	{//if the enemy is close we print the details
		//because enemy is next to you, he will not move but will attack you
		printEnemyDetails(player, zone);

		moveY = 0;
		moveX = 0;
		hitChance = 0; // enemy doesn't move//attack
	}

	//now we check it the enemy is moving in valid spot.
	if (doesHit)
		GM->battle(this);
	else
		checkEnemyMove(player, zone, GM, moveY, moveX);
	return doesHit;
}


void FloorObjects::update(Zone* zone)
{
	int i = rand() % 2;

	curRectText = srcRect[i];
	
	destRect.x = (this->getPosX() * 32) -16;
	destRect.y = (this->getPosY() * 32) -16;
	
	destRect.w = srcRect[i].w;
	destRect.h = srcRect[i].h;
}
void FloorObjects::render(SDL_Renderer& renderer)
{
	SDL_RenderCopy(&renderer, chestTexture, &curRectText, &destRect);
}