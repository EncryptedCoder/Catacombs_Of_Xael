
#include "Enemy.h"




Enemy::Enemy(SDL_Renderer& renderer, std::string& spriteSheet, char &symbol, std::string &name, float &level, float &health, int &endurance, int &money, float& yPos, float& xPos, int &zPos, int &bonusHeal) : Entity(renderer, spriteSheet, symbol, name, level, health, endurance, money, yPos, xPos, zPos)
{
	m_giveBonusHeal = bonusHeal;
}


Enemy::~Enemy()
{
}

//printing enemy details if close to player
void Enemy::printEnemyDetails(Player *player, Zone *zone)
{
	int enemyZ = this->getZpos();
	int enemyY = this->getPosY();
	int enemyX = this->getPosX();

	int playerZ = player->getZpos();
	int playerY = player->getPosY();
	int playerX = player->getPosX();
	
	//if Same Z & Same XY | next to player
	if (enemyZ == playerZ && ((enemyX == playerX && enemyY == playerY) ||
	   (enemyX == playerX && (enemyY + -1 == playerY || enemyY + 1 == playerY)) ||
		(enemyY == playerY && (enemyX + -1 == playerX || enemyX + 1 == playerX))))
	{
		std::cout << this->getName() << "---> " << "Health: " << this->getHealth() << "\n";
	}
	
	
}

//check the move before the move takes place
bool Enemy::checkEnemyMove(Player* player, Zone* zone, GameManager* GM, float& moveY, float& moveX)
{
	bool atkPlayer = false;
	int checkTileAtY = this->getPosY() + moveY;
	int checkTileAtX = this->getPosX() + moveX;
	
	//making sure we will land in a valid tile.
	int tmpZ = this->getZpos();
	float currentPosY = this->getPosY();
	float currentPosX = this->getPosX();

	//int diff = 0;
	//int n = 1;
	int currentWidth = zone->MapWIDTHSx[tmpZ];

	switch (zone->getTile(tmpZ, checkTileAtY, checkTileAtX))
	{
	case Zone::TileType::Water:
		break;
	case Zone::TileType::Dirt:
	case Zone::TileType::Grass:
	case Zone::TileType::StonePath:
		if (this->getSymbol() == (char)233)
		{
			//Plan on making player move this item in same direction as player is moving unless pushing object hits a wall.
		}
		else if (tmpZ == player->getZpos() && 
			(this->getPosY() == player->getPosY() && this->getPosX() == player->getPosX()) && 
			(moveX == 0 && moveY == 0))
		{//if the enemy is under the player we force the enemy to move away, 
			this->move(player, zone, GM); 											
		}
		else if (tmpZ == player->getZpos() && 
			(this->getPosX() == player->getPosX() && (this->getPosY() + moveY == player->getPosY())) || 
			(this->getPosY() == player->getPosY() && (this->getPosX() + moveX == player->getPosX())) &&
			(moveX != 0 && moveY != 0))
		{//makeing sure the enemy attacks if near the player, but only if the move is not 0
			atkPlayer = true;
			return atkPlayer;
		}
		else
		{
			this->setPosY(moveY);
			this->setPosX(moveX);
		}
		break;
	case Zone::TileType::DownStair:
		
		this->setPosY(moveY);
		this->setPosX(moveX);
		this->setZpos(1);
		break;
	case Zone::TileType::UpStair:

		
		this->setPosY(moveY);
		this->setPosX(moveX);
		this->setZpos(-1);
		break;
	default:
		break;
	}
	return 0;
}

bool Enemy::move(Player* player, Zone* zone, GameManager* GM)
{
	int tmpZ = this->getZpos();
	int hitChance = (int)rand() % 100;  //get a random number
	bool doesHit = false;
	
	//checking to see if the enemy is next to m_pPlayer.
   //will eventually add range in place of 1...
	if (tmpZ == player->getZpos() &&
		((this->getPosY() == player->getPosY() &&
		(this->getPosX() + 1 == player->getPosX() || this->getPosX() - 1 == player->getPosX())) ||
		(this->getPosX() == player->getPosX() &&
		(this->getPosY() + 1 == player->getPosY() || this->getPosY() - 1 == player->getPosY()))))
	{//if the enemy is close we print the details
		//because enemy is next to you, he will not move but will attack you
		printEnemyDetails(player, zone);
		if (hitChance < 100) //enemy % chance to hit you...
		{
			doesHit = true;
			this->moveY = 0;
			this->moveX = 0;
		}
		else
			///return something indicating that the enemy isn't attacking.
			doesHit = false;
		hitChance = 0; // enemy doesn't move//attack
	}
	else // if enemy is not next to the Player, it justs moves
	{
		//now need to change hitChance to an x or y coord move.
		if (hitChance < 87)
		{//no move
			//nochange to movement.
		}
		else if (hitChance < 89)
		{//left
			this->moveX = -00.1f;
		}
		else if (hitChance < 91)
		{//right
			this->moveX = 00.1f;
		}
		else if (hitChance < 93)
		{//up
			this->moveY = -00.1f;
		}
		else if (hitChance < 95)
		{//down
			moveY = 00.1f;
		}
		else
		{
			this->moveY = 0;
			this->moveX = 0;
		}
	}
	//now we check it the enemy is moving in valid spot.
	if (doesHit)
		GM->battle(this);
	else
		this->checkEnemyMove(player, zone, GM, moveY, moveX);
	return doesHit;
}
