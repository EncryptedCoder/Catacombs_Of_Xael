#include "FloorObjects.h"

FloorObjects::FloorObjects(SDL_Renderer& renderer, std::string& spriteSheet, std::string& oName, char& symbol, float& level, float& health, int& endurance, int& money, int& pos, int& posZ, int& bonusHeal) :
					Enemy(renderer, spriteSheet, symbol, oName, level, health, endurance, money, pos, posZ, bonusHeal)
{
	chestTexture = TextureManager::LoadTexture(renderer, spriteSheet);
}


FloorObjects::~FloorObjects()
{

}

int FloorObjects::move(Player* player, Zone* zone, GameManager* GM)
{
	int moveChk = 0; 
	if (((this->getPos() + zone->Width[this->getZpos()] == player->getPos())
		|| (this->getPos() - zone->Width[this->getZpos()] == player->getPos())
		|| (this->getPos() + 1 == player->getPos())
		|| (this->getPos() - 1 == player->getPos()))
		&& this->getZpos() == player->getZpos())
	{//if the enemy is close we print the details
		printEnemyDetails(player, zone);
	}
	//now we check it the enemy is moving in valid spot.
	return checkEnemyMove(player, zone, moveChk, GM);
}

void FloorObjects::update(Zone* zone)
{
	//width and height of each spot in a sprite.
	srcRect.h = 32;
	srcRect.w = 32;

	//start of sprite animation
	srcRect.x = 0;
	srcRect.y = 0;

	int mapWidth = zone->Width[this->getZpos()];

	row_y = int(m_pos / mapWidth); //decimal place not needed. dropped...
	col_x = ((m_pos)-(row_y * mapWidth)); //the remainder of this equation will be the col.
	//where sprite will load on map
	destRect.x = col_x * 32;
	destRect.y = row_y * 32;

	destRect.w = srcRect.w;
	destRect.h = srcRect.h;
}
void FloorObjects::render(SDL_Renderer& renderer)
{
	SDL_RenderCopy(&renderer, chestTexture, &srcRect, &destRect);
}