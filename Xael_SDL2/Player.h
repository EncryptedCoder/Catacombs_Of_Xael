#pragma once
#include "GameManager.h"
#include "Enemy.h"
#include "Entity.h"
#include "../SDL2/include/SDL.h"

#include <string>
#include <conio.h>
#include <iostream>
\
class Zone;

class Player : public Entity
{
public://setters and getters // increments
	void setHealthPots(int amount) { m_healthPotions += amount; }
	void setEndurancePots(int amount) { m_endurPotions += amount; }
	void setPlayerDmg(float increaseDmg) { m_playerDmg += increaseDmg; }

	void incrementtimesHealer() { m_timesHealer++; }
	void incrementHealthPotsUsed() { m_hPotsUsed++; }
	void incrementEndurPotsUsed() { m_ePotsUsed++; }
	void incrementKillCount() { m_killCount++; }
	void incrementDeathCount() { m_deathCount++; }

	float getPlayerDmg() { return m_playerDmg; }
	int getHealthPots() { return m_healthPotions; }
	int getEndurancePots() { return m_endurPotions; }
	int getTeleportSpells() { return m_teleportSpells; }

	void setTeleportSpells(int amount) { m_teleportSpells += amount; }
	void incrementTeleportSpellsUsed() { m_teleportSpellsUsed++; }
	int getBaseHealth() { return m_baseHealth; }
	int getBaseEndurance() { return m_baseEndurance; }

	int gettimesHealer()	{ return m_timesHealer++; }
	int getHealthPotsUsed() { return m_hPotsUsed++; }
	int getEndurPotsUsed()	{ return m_ePotsUsed++; }

	int getKillCount() { return m_killCount; }
	int getDeathCount() { return m_deathCount; }

	void toggleMayorFlag() { if (m_mayorChatFlag) m_mayorChatFlag = false; else m_mayorChatFlag = true; }
	bool getMayorFlag() { return m_mayorChatFlag; }


	void setTpBackPosXY() { tpBackPosY = this->m_yPos; tpBackPosX = this->m_xPos; }
	void setTpBackZpos(int zpos) { tpBackZpos = this->m_zPos; }

	float getTpBackPosX() { return tpBackPosX; }
	float getTpBackPosY() { return tpBackPosY; }
	int getTpBackZpos() { return tpBackZpos;}

public://functions
	Player(SDL_Renderer& renderer, std::string& spriteSheetint, bool& loading, std::string &currentMapFile,
				std::string &myName, float& dmg, int& zPos, float& yPos, float& xPos, float &level, int &curHP,
				int &maxHP, int &curEndurance, int &maxEndurance, bool &chatMayor, int &money, 
				int &curHpots, int &curEpots, int &curTeleSpells, int &timesHealer, int &hPotsUsed,
				int &ePotsUsed, int &townTeleUsed, int &kills, int &deaths);
	virtual ~Player();
	void handleEvents(GameManager *GM);
	void printDetails();
	bool move(Player *player, Zone *zone, GameManager *GM) ;
	void save();
	void render(SDL_Renderer& renderer);
	void update(Zone* zone);

private://functions

	void checkPlayerMove(Zone* zone, bool& move, GameManager* GM, float& yMove, float& xMove);
	std::string setName();
	

public://variables	
	SDL_Rect destRect;

private://variables
	//std::string currentMapFilePath;
	
	bool up;
	bool down;
	bool left;
	bool right;
	
	float yMove = 0;
	float xMove = 0;
	int atkY = 0;
	int atkX = 0;

	const float walk = 1;
	const float run = 2.8;

	float speed = 1;

	float tpBackPosY = 0.0f;
	float tpBackPosX = 0.0f;
	int tpBackZpos = 0;
	bool moveTried = false;
	int m_mapWidth=0;
	int m_mapHeight=0;
	bool m_mayorChatFlag = false;
	int m_timesHealer=0;
	int m_hPotsUsed=0;
	int m_ePotsUsed=0;
	int m_teleportSpellsUsed = 0;
	int m_killCount=0;
	int m_deathCount=0;
	int m_healthPotions=0;
	int m_endurPotions=0;
	char m_playerSymbol = '@';
	float m_baseLevel = 1.0f;
	
	float m_yStartPos = 4.0f;
	float m_xStartPos = 10.0f;
	int	m_zStartPos = 0;
	int	m_startMoney = 200;
	float m_playerDmg = .75f;

	SDL_Texture* playerTexture;
	SDL_Rect srcRect;
	

protected:
};

