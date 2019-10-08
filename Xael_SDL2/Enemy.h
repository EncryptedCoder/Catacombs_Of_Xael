#pragma once
#include "Entity.h"
#include "Player.h"

#include <iostream>

class Player;

class Enemy : public Entity
{
protected:
	virtual bool checkEnemyMove(Player* player, Zone* zone, GameManager* GM, float& moveY, float& moveX);

	float moveY = 0.0f;
	float moveX = 0.0f;
public://functions
	Enemy(SDL_Renderer& renderer, std::string& spriteSheet, char &symbol, std::string &name , float &level, float &health, int &endurance, int &money, float& yPos, float& xPos, int &zPos, int &bonusHeal);
	virtual ~Enemy();

	bool move(Player* player, Zone* zone, GameManager* GM);

	void printEnemyDetails(Player* player, Zone* zone);

private://functions




public://variables

	


};