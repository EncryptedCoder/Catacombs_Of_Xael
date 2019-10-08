
#include "Player.h"



class GameManager;

Player::Player(SDL_Renderer& renderer, std::string& spriteSheetint, bool& loading, std::string& currentMapFile,
	std::string& myName, float& dmg,
	int& zPos, float& yPos, float& xPos, float& level, int& curHP, int& maxHP, int& curEndurance, int& maxEndurance,
	bool& chatMayor, int& money, int& curHpots, int& curEpots, int& curTeleSpells, int& timesHealer,
	int& hPotsUsed,	int& ePotsUsed, int& townTeleUsed, int& kills, int& deaths) :
			Entity(renderer, spriteSheetint, m_symbol, a_Name, m_level, m_maxHealth, m_maxEndurance, m_money, m_yPos, m_xPos, m_zPos)
{
	//CURRENTLY SETTING THIS IN ORDER FOR FILE SAVES ECT
		
		if (loading)
		{
			//currentMapFilePath = currentMapFile;
			a_Name = myName;
			m_playerDmg = dmg;
			m_zPos = zPos;
			//m_mapWidth = ;
			//mapH
			m_yPos = yPos;
			m_xPos = xPos;
			m_level = level;
			m_health = curHP;
			m_maxHealth = maxHP;
			m_endurance = curEndurance;
			m_maxEndurance = maxEndurance;
			m_mayorChatFlag = chatMayor;
			m_money = money;
			m_healthPotions = curHpots;
			m_endurPotions = curEpots;
			m_teleportSpells = curTeleSpells;
			m_timesHealer = timesHealer;
			m_hPotsUsed = hPotsUsed;
			m_ePotsUsed = ePotsUsed;
			m_teleportSpellsUsed = townTeleUsed;
			m_killCount = kills;
			m_deathCount = deaths;
		}
		else
		{
			m_mayorChatFlag = false;
			m_level = m_baseLevel;
			m_health = m_baseHealth;
			m_maxHealth = m_baseHealth;
			m_endurance = m_baseEndurance;
			m_maxEndurance = m_baseEndurance;
			m_yPos = m_yStartPos;
			m_xPos = m_xStartPos;
			m_zPos = m_zStartPos;
			m_killCount = 0;
			m_deathCount = 0;
			m_money = m_startMoney;
			m_healthPotions = 0;
			m_endurPotions = 0;
			m_teleportSpells = 0;
			a_Name = setName();
			
		}

	setHpotHealAmount();
	setEpotHealAmount();
	m_symbol = m_playerSymbol;

	playerTexture = TextureManager::LoadTexture(renderer, spriteSheetint);
	destRect.x = (this->getPosX() * 32) -16;//draws in center of the position of char
	destRect.y = (this->getPosY() * 32) -16;//draws near the feet of the char.
}


Player::~Player()
{
}
void Player::update(Zone* zone)
{
	//width and height of each spot in a sprite.
	srcRect.h = 32;
	srcRect.w = 32;

	//start of sprite animation
	srcRect.x = 0;
	srcRect.y = 0;
	
	//where sprite will load on map
	destRect.x = (this->getPosX() * 32) - 16;//draws in center of the position of char
	destRect.y = (this->getPosY() * 32) - 16;//draws near the feet of the char.

	destRect.w = srcRect.w;
	destRect.h = srcRect.h;
}
void Player::render(SDL_Renderer& renderer)
{
	SDL_RenderCopy(&renderer, playerTexture, &srcRect, &destRect);
}
//get player command
bool Player::move(Player* player, Zone* zone, GameManager* GM)
{
	moveTried = true;
	


	if (up)
		yMove = -0.1f * speed;
	else if (down)
		yMove = 0.1f * speed;
	else 
		yMove = 0.0f;

	if (left)
		xMove = -0.1f * speed;
	else if (right)
		xMove = 0.1f * speed;
	else
		xMove = 0.0f;

	checkPlayerMove(zone, moveTried, GM, yMove, xMove);

	

	if (atkY != 0 || atkX != 0)
		GM->playerUpdate(atkY, atkX);
	
	///////!!!!!check this out...
	return true;
}

void Player::handleEvents(GameManager *GM) 
{
	switch (GameManager::events.type)
	{

	case SDL_KEYDOWN:

		if (GameManager::events.key.keysym.sym == SDLK_w)
		{
			up = true;
		}
		if (GameManager::events.key.keysym.sym == SDLK_a)
		{
			left = true;
		}
		if (GameManager::events.key.keysym.sym == SDLK_s)
		{
			down = true;
		}
		if (GameManager::events.key.keysym.sym == SDLK_d)
		{
			right = true;
		}
		if (GameManager::events.key.keysym.sym == SDLK_LSHIFT)
		{
			speed = run;
		}
		if (GameManager::events.key.keysym.sym == SDLK_i)
		{
			atkY = -1.0f;
		}
		if (GameManager::events.key.keysym.sym == SDLK_j)
		{
			atkX = -1.0f;
		}
		if (GameManager::events.key.keysym.sym == SDLK_k)
		{
			atkY = 1.0f;
		}
		if (GameManager::events.key.keysym.sym == SDLK_l)
		{
			atkX = 1.0f;
		}
		if (GameManager::events.key.keysym.sym == SDLK_p)
		{
			GM->pauseGame(this, GM);
			//GM->togglePause();
		}
			
			break;
	case SDL_KEYUP:

		if (GameManager::events.key.keysym.sym == SDLK_w)
		{
			up = false;
		}
		if (GameManager::events.key.keysym.sym == SDLK_a)
		{
			left = false;
		}
		if (GameManager::events.key.keysym.sym == SDLK_s)
		{
			down = false;
		}
		if (GameManager::events.key.keysym.sym == SDLK_d)
		{
			right = false;
		}
		if (GameManager::events.key.keysym.sym == SDLK_LSHIFT)
		{
			speed = walk;
		}
		if (GameManager::events.key.keysym.sym == SDLK_i)
		{
			atkY = -0.0f;
		}
		if (GameManager::events.key.keysym.sym == SDLK_j)
		{
			atkX = -0.0f;
		}
		if (GameManager::events.key.keysym.sym == SDLK_k)
		{
			atkY = 0.0f;
		}
		if (GameManager::events.key.keysym.sym == SDLK_l)
		{
			atkX = 0.0f;
		}
		break;
	default:
		break;
	}
}

//checking movement
void Player::checkPlayerMove(Zone* zone, bool& canMove, GameManager* GM, float& yMove, float& xMove)
{
	float currentPos_Y = this->getPosY();
	float currentPos_X = this->getPosX();
	float checkTileAt_Y = currentPos_Y + yMove;
	float checkTileAt_X = currentPos_X + xMove;
	int tmpZ = this->getZpos();
	//making sure we will land in a valid tile.
	
	
	if (canMove && !((checkTileAt_Y >= zone->MapHEIGHTSy[tmpZ] || checkTileAt_X >= zone->MapWIDTHSx[tmpZ]) || (checkTileAt_Y < 0 || checkTileAt_X < 0 )))
	{
		switch (zone->getTile(tmpZ, checkTileAt_Y, currentPos_X))
		{
		case Zone::TileType::Water:
			break;
		case Zone::TileType::Dirt:
		case Zone::TileType::StonePath:
		case Zone::TileType::Grass:
			this->setPosY(yMove);
			break;
		case Zone::TileType::DownStair://///////////!!!!!!!!!!!!
			system("cls||clear");//clear screen so everything can be redrawn to match current map size.
			this->setPosY(yMove);
			this->setZpos(1);
			break;
		case Zone::Zone::TileType::UpStair://clear is for the console before graphics. ///////////!!!!!!!!!!!
			system("cls||clear");//clear screen so everything can be redrawn to match current map size.
			this->setPosY(yMove);
			this->setZpos(-1);
			break;
		case Zone::TileType::LRdoor:
			if (this->m_mayorChatFlag)
			{
				this->setPosY(yMove);
			}
			else
				GameManager::dungeonDoor(this, this->m_mayorChatFlag);
			break;
		default:
			break;
		}

		switch (zone->getTile(tmpZ, currentPos_Y, checkTileAt_X))
		{
		case Zone::TileType::Water:
			break;
		case Zone::TileType::Dirt:
		case Zone::TileType::StonePath:
		case Zone::TileType::Grass:
			this->setPosX(xMove);
			break;
		case Zone::TileType::DownStair://///////////!!!!!!!!!!!!
			system("cls||clear");//clear screen so everything can be redrawn to match current map size.
			
			this->setPosX(xMove);
			this->setZpos(1);
			break;
		case Zone::Zone::TileType::UpStair://clear is for the console before graphics. ///////////!!!!!!!!!!!
			system("cls||clear");//clear screen so everything can be redrawn to match current map size.
			
			this->setPosX(xMove);
			this->setZpos(-1);
			break;
		case Zone::TileType::LRdoor:
			if (this->m_mayorChatFlag)
			{
				
				this->setPosX(xMove);
			}
			else
				GameManager::dungeonDoor(this, this->m_mayorChatFlag);
			break;
		default:
			break;
		}
		
	}
}

//prints the players details
void Player::printDetails()
{
	std::cout << this->a_Name << "-> " << "Health: " << this->m_health << "/" << this->m_maxHealth << " | Endurance: " << this->m_endurance << "/" << this->m_maxEndurance <<
		"\n\tLvL: " << (int)this->getLevel() << " | Exp: " << int((this->getLevel() - (int)this->getLevel()) * 100) << "%\n"
		"\tH-Potion: " << this->m_healthPotions << " | E-Potion: " << this->m_endurPotions
		<< "\n\tMoney: " << this->m_money << "\tKill/Death Ratio: " << this->m_killCount << "/" << this->m_deathCount << '\n';
}

//create name of player

std::string Player::setName()
{ //Don't know if this helps any, but only use what is needed.
	std::string naming;
	bool stillNaming = false;
	do {
	std::cout << "\n\n\n\n\nPlease enter a name for your character!\n\n\n\n";

	std::getline(std::cin, naming);
	
		if (naming == "" || naming == "Pickle")
		{
			naming = "Pickle";
			std::cout << "You entered nothing! I'm naming you '" << naming << "'!\n\n\n";
			std::cout << "If you would like to keep this name press 0, otherwise press 1 to change.";
			std::cin >> stillNaming;
			std::cin.ignore(256, '\n');
		}
	} while (stillNaming);
	return naming;
}

void Player::save()
{
	std::ofstream saveFile;
	saveFile.open("assets/MapFiles/CharSaves/" + a_Name + ".txt");

	//saveFile << currentMapFilePath << ' ';
	saveFile << a_Name << ' ';
	saveFile << m_playerDmg << ' ';
	saveFile << m_zPos << ' ';
	saveFile << m_yPos << ' ';
	saveFile << m_xPos << ' ';
	saveFile << m_level << ' ';
	saveFile << m_health << ' ';
	saveFile << m_maxHealth << ' ';
	saveFile << m_endurance << ' ';
	saveFile << m_maxEndurance << ' ';
	saveFile << m_mayorChatFlag << ' ';
	saveFile << m_money << ' ';
	saveFile << m_healthPotions << ' ';
	saveFile << m_endurPotions << ' ';
	saveFile << m_teleportSpells << ' ';
	saveFile << m_timesHealer << ' ';
	saveFile << m_hPotsUsed << ' ';
	saveFile << m_ePotsUsed << ' ';
	saveFile << m_teleportSpellsUsed << ' ';
	saveFile << m_killCount << ' ';
	saveFile << m_deathCount << ' ';
	saveFile.close();
}