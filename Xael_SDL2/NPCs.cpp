#include "NPCs.h"





NPCs::NPCs(SDL_Renderer& renderer, std::string& spriteSheet, std::string& npcname, char& symbol, float& level, float& health, int& endurance, int& money, float& yPos, float& xPos, int& zPos) :
					Entity(renderer, spriteSheet,symbol, npcname, level, health, endurance, money, yPos, xPos, zPos)
{
	npcTextures = TextureManager::LoadTexture(renderer, spriteSheet);
	
	animFrame = (rand() % 4);
	slowDowFrame = rand() % 4 + rand() % 8;
	
	//width and height of each spot in a sprite.
	srcRect[0].h = 32;
	srcRect[0].w = 24;
	//start of sprite animation
	srcRect[0].x = 24;
	srcRect[0].y = 64;

	//width and height
	srcRect[1].h = 32;
	srcRect[1].w = 24;
	//2nd sprite animation
	srcRect[1].x = 24;
	srcRect[1].y = 32;

	//width and height.
	srcRect[2].h = 32;
	srcRect[2].w = 24;
	//4th sprite animation
	srcRect[2].x = 24;
	srcRect[2].y = 64;

	//width and height
	srcRect[3].h = 32;
	srcRect[3].w = 24;
	//3rd sprite animation
	srcRect[3].x = 24;
	srcRect[3].y = 96;

	//where sprite will load on map
	destRect.x = posX * 32;
	destRect.y = posY * 32;
	//new width and height
	destRect.w = srcRect[0].w;
	destRect.h = srcRect[0].h;

	currentFrame = srcRect[animFrame / 4];
}


NPCs::~NPCs()
{
	SDL_DestroyTexture(npcTextures);
}

void NPCs::npcDetails(Player* player, Zone* zone, GameManager* GM)
{

	if (player->getZpos() == this->getZpos() && GM->checkCollision(player->destRect, this->destRect))
	{
		std::cout << '\n' << this->getName() << "\n";
	}
}

bool NPCs::move(Player* player, Zone* zone, GameManager* GM)
{
	return 0;
}

void NPCs::updatePortal(Zone* zone)
{
	int mapWidth = zone->MapWIDTHSx[this->getZpos()];
	
	currentFrame = srcRect[rand() % 4];

	//where sprite will load on map
	destRect.x = (this->getPosX() * 32) - 12;
	destRect.y = (this->getPosY() * 32) - 16;

	destRect.w = currentFrame.w;
	destRect.h = currentFrame.h;
}

void NPCs::update(Zone* zone)
{
	int mapWidth = zone->MapWIDTHSx[this->getZpos()];
	if (!(slowDowFrame % 14))
	{
		animFrame++;
	}
	if (animFrame / 4 >= 4)
	{
		animFrame = rand() % 4;
		slowDowFrame = rand() % 100;
	}
	currentFrame = srcRect[animFrame / 4];

	//where sprite will load on map
	destRect.x = (this->getPosX() * (32 - 2));
	destRect.y = (this->getPosY() * 32) - 16;

	destRect.w = currentFrame.w;
	destRect.h = currentFrame.h;

	slowDowFrame++;
	
}
void NPCs::render(SDL_Renderer& renderer)
{
	SDL_RenderCopy(&renderer, npcTextures, &currentFrame, &destRect);
}