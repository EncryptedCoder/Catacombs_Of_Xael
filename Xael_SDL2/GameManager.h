#pragma once
#include <string>
#include <cmath>
#include <iostream>
#include <string>
#include <conio.h>
#include <thread>
#include <chrono>
#include "../SDL2/include/SDL.h"
#include "../SDL2/include/SDL_render.h"
#include "GameManager.h" 


class Zone;
class Entity;
class Player;
class Enemy;
class Skeleton;
class Mummy;
class Zombie;
class Boss;
class FloorObjects;
class NPCs;


class GameManager
{
	

public:
	GameManager();
	virtual ~GameManager();
	void run();
	void init();
	void handleEvents();
	void update();
	void render(Player *player);
	bool running();
	void togglePause() { if (isGamePaused)isGamePaused = true; else isGamePaused = false; }


	static SDL_Event events;

	static const int mk_maxNumbOfRats = 30;
	static const int mk_maxNumOfSkellies = 20;
	static const int mk_maxNumOfMummies = 20;
	static const int mk_maxNumOfZombies = 15;
	static const int mk_maxNumOfBosses = 10;
	static const int mk_maxNumOfFloorObjects = 4;

	static void pauseGame(Player *player, GameManager *GM);

	void createPortals();
	void createNPCs();
	void createSkellies();
	void createMummies();
	void createZombies();
	void createBosses();
	void createFloorObjects();

	static void dungeonDoor(Player *player, bool &mayerFlag);
	static void mayorSpeach(Player *player);
	static void shopping(Player* player);

	void playerUpdate(int& atkY, int& atkX);

	void createEnemies();

	bool battle(Entity  *enemy);
	bool checkCollision(SDL_Rect& a, SDL_Rect& b);

private:
	void createPlayer();
	//this will be used more when i make the game larger..
	enum MapName { Tutorial = 0, Act1, Act2, NumberOfMaps };

	std::string _mapPath[NumberOfMaps];
	int mapIndex = 0;
	
	Player *m_pPlayer = nullptr;

	NPCs* m_pShopKeeper = nullptr;
	NPCs* m_pMayor = nullptr;
	NPCs* m_pHealer = nullptr;
	NPCs** m_pPortal = nullptr;

	Skeleton** m_pRats = nullptr;
	Skeleton** m_pSkellies = nullptr;

	Mummy** m_pMummies = nullptr;
	Zombie **m_pZombies = nullptr;
	Boss **m_pBosses = nullptr;
	FloorObjects** m_pFloorObjects = nullptr;

	//file names for sprite char...


	std::string myPlayerSpriteFilePath = "assets/oddGuy.png";

	std::string spriteFileMayor = ("assets/Townfolk-Old-F.png"); //init and randomize skellies
	std::string spriteFileShop = ("assets/Townfolk-Adult-F.png"); //init and randomize skellies
	std::string spriteFileHealer = ("assets/priest.png"); //init and randomize skellies

	std::string spriteFilePortal = ("assets/map_tiles/Props/m_flush_Lisa.png");
	std::string spriteFileO = ("assets/map_tiles/Props/chestSparkle__Lisa.png"); //init and randomize floorobjects

		


	std::string spriteFileS = ("assets/skeleton.png"); //init and randomize skellies
	std::string spriteFileM = ("assets/mummy.png"); //init and randomize mummies
	std::string spriteFileZ = ("assets/zombie.png"); //init and randomize zombies
	std::string spriteFileB = ("assets/reaper.png"); //init and randomize bosses
	std::string spriteRATS = ("assets/rats.png");
	

	bool portalActive;
	bool isRunning = false;
	//creating pointers to SDL Window & Renderer for use and reuse.
	SDL_Window* sdl_pWindow = nullptr;
	SDL_Renderer* winRenderer = nullptr;

	void gainedChat(std::string gainedWhat, float howMuch, std::string thenWhat, float howMuch2, bool updateText);

	
	void updatePlayerStats(Player *player, Entity *enemy);


	void skellyUpdate();
	void mummyUpdate();
	void zombieUpdate();
	void bossUpdate();
	void floorObjectsUpdate();

	
	void ratsUpdate();

	void printWelcome(Player *player);
	void checkWin();

	float randPosition(int zPos);
	int randZlevel();

	enum EntityTurn {PlayersTurn, SkelliesTurn, ZombiesTurn, MummiesTurn, BossesTurn, FlrObjTurn, RatsTurn};
	int whosTurn = PlayersTurn;
	
	std::string gained1 = "";// for gainedChat()...
	std::string gained2 = "";// for gainedChat()...
	float howMuchOfGained1 = 0;
	float howMuchOfGained2 = 0;

	bool isGameOver = false;
	bool isValidMove = true;
	bool isEnemyDead = false;
	bool isGamePaused = false;

	float portalInTownPosX = 11;
	float portalInTownPosY = 14;

	std::string m_mayerName = "Mayor Nindia";
	char m_mayorSymbol = 'M';
	float m_mayorPosY = 14;
	float m_mayorPosX = 13;
	int m_mayorZpos = 0;

	std::string m_shopName = "Shop Keeper Valyri";
	char m_shopSymbol = '$';
	float m_shopPosY = 7;
	float m_shopPosX = 5;
	int m_shopZpos = 0;

	std::string m_healerName = "Healer Palious";
	char m_healerSymbol = 'H';
	float m_healerPosY = 8;
	float m_healerPosX = 13;
	int m_healerZpos = 0;

	std::string m_portalName = "Portal back to the catacombs!";
	char m_portalSymbol = 'p';
	float m_portalPosY = this->portalInTownPosY;
	float m_portalPosX = this->portalInTownPosX;
	int m_portalZpos = 0;

	float m_NPCHealth = 10.0f;
	int m_NPCEndurance = 25;
	float m_NPCLevel = 1.0f;
	int m_NPCMoney = 0;


	std::string m_ratsName = "Nasty Rats";
	int m_ratsCount = 0;
	int m_ratsKillCount = 0;
	float m_ratsHealth = 4.5f;
	int m_ratsEndurance = 2;
	char m_ratsSymbol = 'r';
	float m_ratsLevel = .3f;
	int m_ratsBonusHeal = 1;
	
	

	std::string m_sName = "Skeleton";
	int m_skellyCount = 0;
	int m_skellyKillCount = 0;
	float m_skellyHealth = 10.0f;
	int m_skellyEndurance = 25;
	char m_skellySymbol = 'S';
	float m_skellyLevel = 1.0f;
	int m_skellyBonusHeal = 2;


	std::string m_mName = "Mummy";
	int	m_mummyCount = 0;
	int	m_mummyKillCount = 0;
	float m_mummyHealth = 40.0f;
	int	m_mummyEndurance = 30;
	char m_mummySymbol = '&';
	float m_mummyLevel = 3.0f;
	int	m_mummyBonusHeal = 8;

	std::string m_zName = "Zombie";
	int m_zombieCount = 0;
	int m_zombieKillCount = 0;
	float m_zombieHealth = 150.0f;
	int m_zombieEndurance = 50;
	char m_zombieSymbol = 'Z';
	float m_zombieLevel = 6.0f;
	int m_zombieBonusHeal = 18;


	std::string m_bossName = "The Boss";
	int m_bossCount = 0;
	int m_bossKillCount = 0;
	float m_bossHealth = 2000.0f;
	int m_bossEndurance = 400;
	char m_bossSymbol = 'B';
	float m_bossLevel = 18.0f;
	float m_bossZpos = 7.0f;
	int m_bossBonusHeal = 300;

	std::string m_objName = "Chest";
	int m_flrObjCount = 0;
	int m_flrObjFound = 0;
	int m_flrObjKillCount = 0;
	float m_flrObjHealth = 3.0f;
	int m_flrObjEndurance = 0;
	char m_flrObjSymbol = char(233); // char is 'Θ'
	float m_flrObjLevel = 0.0f;
	int m_flrObjBonusHeal = 2;

	float m_attackDirection = 0.0f;


	static const int mk_healthPotPrice = 520;
	static const int mk_endurancePotPrice = 210;
	static const int mk_teleportTownPrice = 75;
	static const int mk_weaponUpgradePrice = 1150;

	float randyPos;
	float randxPos;
};