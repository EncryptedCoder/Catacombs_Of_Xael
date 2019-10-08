#include "Skeleton.h"
#include "Mummy.h"
#include "Zombie.h"
#include "Boss.h"
#include "NPCs.h"
#include "Player.h"
#include "FloorObjects.h"
#include "Window.h"

#include "GameManager.h"


Window* cwindow = nullptr;
Zone* gZone = nullptr;


SDL_Event GameManager::events;

GameManager::GameManager()
{//plan to make multipul Acts
	
	isRunning = true;
	isGameOver = false;
	this->portalActive = false;

	_mapPath[Tutorial] = "assets/MapFiles/Tutorial.txt";
	_mapPath[Act1] = "assets/MapFiles/Map_1.txt";
	_mapPath[Act2] = "assets/MapFiles/Map_2.txt";

	m_pPortal = new NPCs * [2];
	m_pSkellies = new Skeleton * [mk_maxNumOfSkellies];
	m_pMummies = new Mummy * [mk_maxNumOfMummies];
	m_pZombies = new Zombie * [mk_maxNumOfZombies];
	m_pBosses = new Boss * [mk_maxNumOfBosses];
	m_pFloorObjects = new FloorObjects * [mk_maxNumOfFloorObjects];
	m_pRats = new Skeleton * [mk_maxNumbOfRats];

	m_pPortal[0] = nullptr;
	m_pPortal[1] = nullptr;

	for (int i = 0; i < mk_maxNumOfSkellies; i++)
	{  // set all skellies to nullptr
		m_pSkellies[i] = nullptr;
	}
	for (int i = 0; i < mk_maxNumOfMummies; i++)
	{  // set all mummies to nullptr
		m_pMummies[i] = nullptr;
	}
	for (int i = 0; i < mk_maxNumOfZombies; i++)
	{  // set all zombies to nullptr
		m_pZombies[i] = nullptr;
	}
	for (int i = 0; i < mk_maxNumOfBosses; i++)
	{  // set all Bosses to nullptr
		m_pBosses[i] = nullptr;
	}
	for (int i = 0; i < mk_maxNumOfFloorObjects; i++)
	{  // set all FloorObjects to nullptr
		m_pFloorObjects[i] = nullptr;
	}
	for (int i = 0; i < mk_maxNumbOfRats; i++)
	{  // set all FloorObjects to nullptr
		m_pRats[i] = nullptr;
	}
	
};
GameManager::~GameManager() 
{
	delete m_pPlayer;
	delete[] *m_pSkellies;
	delete[] *m_pMummies;
	delete[] *m_pZombies;
	delete[] *m_pBosses;
	delete[] *m_pFloorObjects;
	delete[] m_pHealer;
	delete[] m_pMayor;
	delete[] m_pShopKeeper;
	if(*m_pPortal != nullptr)
		delete[] * m_pPortal;

	//deletes renderer
	SDL_DestroyRenderer(winRenderer);

};

//The main game Loop
void GameManager::run()
{
	isGameOver = false;
	bool isBattleTime = false;
	using namespace std::this_thread;     // sleep_for, sleep_until
	using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
	//using std::chrono::system_clock;
	

	//FPS I want to keep game at.
	const int FPS = 20;
	//
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;
	
	std::cout << "Which map would you like?\n\n"
		"0: Tutorial\n1: Map_1\n2: Map_2\n\n";
	std::cin >> mapIndex;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	gZone = new Zone(_mapPath[mapIndex]);
	init();
	printWelcome(m_pPlayer);
	//game loop
	while (running()) 
	{
		//counter start in time miliseconds
		frameStart = SDL_GetTicks();

		//handles keyboard events and player movement.
		handleEvents();
		

		if (!isGamePaused)
		{
			//print map to the console area
			gZone->printZone(m_pPlayer, m_pSkellies, m_pRats ,m_pMummies, m_pZombies, m_pBosses, m_pFloorObjects, m_pHealer, m_pShopKeeper, m_pMayor); //printing the map
			printf("\033[%d;%dH", 1, 1);
			m_pPlayer->printDetails();  // printing player details
			printf("\033[%d;%dH", 6, 1);
			std::cout << "                                    \n";
			std::cout << "                                    \n";
			std::cout << "                                    \n";
			std::cout << "                                    \n";
			std::cout << "                                    \n";
			gainedChat("", 0, "", 0, false);
			printf("\033[%d;%dH",6, 1);
			//updates all other entities
			update();
			/////////PRINTING DETAILS OF NPC CLASS
			m_pMayor->npcDetails(m_pPlayer, gZone, this);
			m_pShopKeeper->npcDetails(m_pPlayer, gZone, this);
			m_pHealer->npcDetails(m_pPlayer, gZone, this);
			
			//drawing to the screen
			render(m_pPlayer);
			//solely checks to see if a boss exsists, if no boss, game complete.
			checkWin();
		}
		
		
		//time between each update of the game
		frameTime = SDL_GetTicks() - frameStart;

		//check to see if we need to delay next frame
		if (frameDelay >= frameTime)
		{
			//delay frames
			SDL_Delay(frameDelay - frameTime);
		}
	}
}
bool GameManager::running()
{
	if (cwindow->running())
	{
		return isRunning;
	}
	return false;

}


void GameManager::init()
{
	bool fullscreen = false;
	int width = 0, height = 0;
	cwindow = new Window(width, height, gZone);
	cwindow->init(_mapPath[mapIndex]);
	//create the Renderer
	if (winRenderer == nullptr)
		if (!(winRenderer = SDL_CreateRenderer(cwindow->sdl_pWindow, -1, SDL_RENDERER_ACCELERATED)))
		{
			std::cerr << "CreateRenderer failed!" << std::endl;
			//isRunning = false;
			isRunning = false;
		}
		else
		{
			SDL_SetRenderDrawColor(winRenderer, 0, 0, 0, 255);
			isRunning = true;//wasn't here before changes
		}
	gZone->initMapTextures(*winRenderer);

	

	//CREATING PLAYER
	createPlayer();
	
	//CREATING NPCs
	createNPCs();

	//CREATING ENEMIES	
	createEnemies();
}

void GameManager::createPlayer()
{
	//tempory player variables to pass into creating player...
	std::string currentActMap; std::string strName;	float dmg; int zPos;
	float yPos; float xPos; float level; int health; int maxHealth; int endurance; int maxEndurance; bool mayorChatFlag;
	int money; int healthPotions; int endurPotions; int teleportSpells;	int timesHealer; int hPotsUsed;
	int ePotsUsed; int teleportSpellsUsed; int killCount; int deathCount; bool loadSave = false;
	char c = 'b';
	do
	{
		std::cout << "\nWould you like to load from a file?\n"
			"\t\tyes / no\n"
			"\t\t (1 or 0)\t";
		std::cin >> loadSave;
		std::cin.ignore(256, '\n');
	} while (std::cin.fail());
	if (loadSave)
	{
		std::ifstream file;
		std::string fileFolder = "assets/MapFiles/CharSaves/";
		std::string saveFileName;

		while (loadSave)
		{
			std::cout << "\n\n\tCan you tell me the name you used for\n"
				"your character last you played? This will be\n"
				"the name of your saved file.\n\n";

			std::getline(std::cin, saveFileName);
			saveFileName = fileFolder + saveFileName + ".txt";
			file.open(saveFileName);
			if (file.fail())
			{
				
				perror(saveFileName.c_str());
				std::cout << "\n\nTry again?\ny or n\n";
				std::cin >> c;
				std::cin.ignore(256, '\n');

				if (c != 'y')
					loadSave = false;
			}
			else
				loadSave = false;
		}
		if(c != 'n')
			loadSave = true;

		//file >> currentActMap;
		file >> strName;
		file >> dmg;
		file >> zPos;
		file >> yPos;
		file >> xPos;
		file >> level;
		file >> health;
		file >> maxHealth;
		file >> endurance;
		file >> maxEndurance;
		file >> mayorChatFlag;
		file >> money;
		file >> healthPotions;
		file >> endurPotions;
		file >> teleportSpells;
		file >> timesHealer;
		file >> hPotsUsed;
		file >> ePotsUsed;
		file >> teleportSpellsUsed;
		file >> killCount;
		file >> deathCount;
		file.close();
	}

	m_pPlayer = new Player(*winRenderer, myPlayerSpriteFilePath, loadSave, currentActMap,
		strName, dmg, zPos, yPos, xPos, level, health, maxHealth, endurance, maxEndurance,
		mayorChatFlag, money, healthPotions, endurPotions, teleportSpells, timesHealer,
		hPotsUsed, ePotsUsed, teleportSpellsUsed, killCount, deathCount);
}

void GameManager::createNPCs()
{
	m_pMayor = new NPCs(*winRenderer, spriteFileMayor, m_mayerName, m_mayorSymbol, m_NPCLevel, m_NPCHealth, m_NPCEndurance, m_NPCMoney, m_mayorPosY, m_mayorPosX, m_mayorZpos);
	m_pShopKeeper = new NPCs(*winRenderer, spriteFileShop, m_shopName, m_shopSymbol, m_NPCLevel, m_NPCHealth, m_NPCEndurance, m_NPCMoney, m_shopPosY, m_shopPosX, m_shopZpos);
	m_pHealer = new NPCs(*winRenderer, spriteFileHealer, m_healerName, m_healerSymbol, m_NPCLevel, m_NPCHealth, m_NPCEndurance, m_NPCMoney, m_healerPosY, m_healerPosX, m_healerZpos);
}

void GameManager::createPortals()
{
	
	if (m_pPortal != nullptr)
		delete[] m_pPortal;
	
	m_pPortal = new NPCs*[2];
	//portal 0 is always in town
	m_pPortal[0] = new NPCs(*winRenderer, spriteFilePortal, m_portalName, m_portalSymbol, m_NPCLevel, m_NPCHealth, m_NPCEndurance, m_NPCMoney, m_portalPosY, m_portalPosX, m_portalZpos);
	
	//portal 1 needs data from players current location to set its location.
	float portalPosY = m_pPlayer->getPosY();
	float portalPosX = m_pPlayer->getPosX();
	int portalZpos = m_pPlayer->getZpos();
	m_pPortal[1] = new NPCs(*winRenderer, spriteFilePortal, m_portalName, m_portalSymbol, m_NPCLevel, m_NPCHealth, m_NPCEndurance, m_NPCMoney, portalPosY, portalPosX, portalZpos);
}

void GameManager::createEnemies()
{
	createSkellies(); //init and randomize skellies
	createMummies(); //init and randomize mummies
	createZombies(); //init and randomize zombies
	createBosses(); //init and randomize bosses
	createFloorObjects(); //init and randomize floorobjects
}

void GameManager::handleEvents()
{
	whosTurn = PlayersTurn;
	while (SDL_PollEvent(&events))
	{
		m_pPlayer->handleEvents(this);
		cwindow->handleEvents();
	}
}


void GameManager::gainedChat(std::string gainedWhat, float howMuch, std::string thenWhat, float howMuch2, bool updateText)
{
	if (updateText)
	{
		gained1 = gainedWhat;
		gained2 = thenWhat;
		howMuchOfGained1 = howMuch;
		howMuchOfGained2 = howMuch2;
	}
	
	printf("\033[%d;%dH", 13, 1);
	std::cout << "You gain '" << howMuchOfGained1 << "' " + gained1;
	std::cout << "\nYou gain '" << howMuchOfGained2 << "' " + gained2;
}


void GameManager::update()
{
	m_pPlayer->move(m_pPlayer, gZone, this);
	m_pPlayer->update(gZone);
	m_pMayor->update(gZone);
	m_pShopKeeper->update(gZone);
	m_pHealer ->update(gZone);
	if (this->portalActive)
	{
		m_pPortal[0]->updatePortal(gZone);
		m_pPortal[1]->updatePortal(gZone);
	}
	skellyUpdate();
	mummyUpdate();
	zombieUpdate();
	bossUpdate();
	floorObjectsUpdate();
	ratsUpdate();
}
void GameManager::render(Player *player)
{///Render order, first - last. last will show front of other objects.
	int curZpos = player->getZpos();

	SDL_RenderClear(winRenderer);
	//drawing map tiles...
	cwindow->render(winRenderer, curZpos);
	
	if (this->portalActive && *m_pPortal != nullptr && curZpos == m_pPortal[0]->getZpos())
	{		
		m_pPortal[0]->render(*winRenderer);
	}
	else if (this->portalActive && *m_pPortal != nullptr && curZpos == m_pPortal[1]->getZpos())
	{
		m_pPortal[1]->render(*winRenderer);
	}

	m_pPlayer->render(*winRenderer);

	//render only if these are on same Z level as myself.
	if (curZpos == 0)//in town
	{
		m_pMayor->render(*winRenderer);
		m_pShopKeeper->render(*winRenderer);
		m_pHealer->render(*winRenderer);

	}
	
	
		for (int i = 0; i < mk_maxNumbOfRats; i++)
		{
			if (m_pRats[i] != nullptr && m_pRats[i]->getZpos() == curZpos)
				m_pRats[i]->render(*winRenderer);
		}
		for (int i = 0; i < mk_maxNumOfSkellies; i++)
		{
			if (m_pSkellies[i] != nullptr && m_pSkellies[i]->getZpos() == curZpos)
				m_pSkellies[i]->render(*winRenderer);
		}
		for (int i = 0; i < mk_maxNumOfMummies; i++)
		{
			if (m_pMummies[i] != nullptr && m_pMummies[i]->getZpos() == curZpos)
				m_pMummies[i]->render(*winRenderer);
		}
		for (int i = 0; i < mk_maxNumOfZombies; i++)
		{
			if (m_pZombies[i] != nullptr && m_pZombies[i]->getZpos() == curZpos)
				m_pZombies[i]->render(*winRenderer);
		}
		for (int i = 0; i < mk_maxNumOfBosses; i++)
		{
			if (m_pBosses[i] != nullptr && m_pBosses[i]->getZpos() == curZpos)
				m_pBosses[i]->render(*winRenderer);
		}
		for (int i = 0; i < mk_maxNumOfFloorObjects; i++)
		{
			if (m_pFloorObjects[i] != nullptr && m_pFloorObjects[i]->getZpos() == curZpos)
				m_pFloorObjects[i]->render(*winRenderer);
		}
		
	
		
	//place rendered things in window.
	SDL_RenderPresent(winRenderer);
}
bool GameManager::checkCollision(SDL_Rect &a, SDL_Rect &b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
							   //topa
	leftA = a.x;			 //lefta|--5--|righta
	rightA = a.x + a.w;		 //     3     4
	topA = a.y;				 //     |     |
	bottomA = a.y + a.h;	 //botma|__6__|
	
	//Calculate the sides of rect B
								//topb
	leftB = b.x;			  //leftb|-5.5-|rightb
	rightB = b.x + b.w;		  //     |     |
	topB = b.y;				  //     2     3
	bottomB = b.y + b.h;	  //botmb|_6.5_|
	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}
//getting and updating the players move
void GameManager::playerUpdate(int& atkY, int& atkX)
{
	
	if (atkY != 0 || atkX != 0)
	{

		//Based only if in town...
		if (m_pPlayer->getZpos() == 0)
		{//m_pPlayer
			if (checkCollision(m_pPlayer->destRect, m_pMayor->destRect))
			{
				GameManager::mayorSpeach(m_pPlayer);
				if (!m_pPlayer->getMayorFlag())
					m_pPlayer->toggleMayorFlag(); //we have spoke to the mayer at least once now.
			}
			else if (checkCollision(m_pPlayer->destRect, m_pHealer->destRect))
			{//m_pHealer
				if (m_pPlayer->getHealth() < m_pPlayer->getMaxHealth() || m_pPlayer->getEndurance() < m_pPlayer->getMaxEndurance())
				{
					m_pPlayer->setHealth(-m_pPlayer->getHealth() + m_pPlayer->getMaxHealth());
					m_pPlayer->setEndurance(-m_pPlayer->getEndurance() + m_pPlayer->getMaxEndurance());
					m_pPlayer->incrementtimesHealer();
				}
				this->createEnemies();
			}
			if (checkCollision(m_pPlayer->destRect, m_pShopKeeper->destRect))
			{//m_pShopKeeper
				shopping(m_pPlayer);
			}

			if (this->portalActive && 
				(checkCollision(m_pPlayer->destRect, m_pPortal[0]->destRect)))
			{//m_pPortal[0]
				this->portalActive = false;
				m_pPlayer->setZpos(m_pPortal[1]->getZpos());
				m_pPlayer->setPosY(-m_pPlayer->getPosY() + m_pPortal[1]->getPosY());
				m_pPlayer->setPosX(-m_pPlayer->getPosX() + m_pPortal[1]->getPosX());
				delete[] * m_pPortal;
				*m_pPortal = nullptr;

			}

		}
		else
		{
			if (this->portalActive &&
				( m_pPlayer->getZpos() == m_pPortal[1]->getZpos() && 
					checkCollision(m_pPlayer->destRect, m_pPortal[1]->destRect)))
			{//portal
				m_pPlayer->setZpos(-m_pPlayer->getZpos());
				m_pPlayer->setPosY(-m_pPlayer->getPosY() + this->portalInTownPosY);
				m_pPlayer->setPosX(-m_pPlayer->getPosX() + this->portalInTownPosX);
			}

			for (int i = 0; i < mk_maxNumbOfRats; i++)
			{
				if (i < mk_maxNumOfSkellies && m_pSkellies[i] != nullptr && m_pPlayer->getZpos() == m_pSkellies[i]->getZpos() &&
					(m_pPlayer->getZpos() == m_pSkellies[i]->getZpos() &&	checkCollision(m_pPlayer->destRect, m_pSkellies[i]->destRect)))
				{
					if (battle(m_pSkellies[i]))
					{
						delete m_pSkellies[i];
							m_pSkellies[i] = nullptr;
							m_skellyCount--;
							m_skellyKillCount++;
					}
				}
				if (i < mk_maxNumOfMummies && m_pMummies[i] != nullptr && m_pPlayer->getZpos() == m_pMummies[i]->getZpos() &&
					(m_pPlayer->getZpos() == m_pMummies[i]->getZpos() && checkCollision(m_pPlayer->destRect, m_pMummies[i]->destRect)))
				{
					if (battle(m_pMummies[i]))
					{
						delete m_pMummies[i];
						m_pMummies[i] = nullptr;
						m_mummyCount--;
						m_mummyKillCount++;
					}
				}
				if (i < mk_maxNumOfZombies && m_pZombies[i] != nullptr && m_pPlayer->getZpos() == m_pZombies[i]->getZpos() &&
					(m_pPlayer->getZpos() == m_pZombies[i]->getZpos() && checkCollision(m_pPlayer->destRect, m_pZombies[i]->destRect)))
				{
					if (battle(m_pZombies[i]))
					{
						delete m_pZombies[i];
						m_pZombies[i] = nullptr;
						m_zombieCount--;
						m_zombieKillCount++;
					}
				}
				if (i < mk_maxNumOfBosses && m_pBosses[i] != nullptr && m_pPlayer->getZpos() == m_pBosses[i]->getZpos() &&
					(m_pPlayer->getZpos() == m_pBosses[i]->getZpos() && checkCollision(m_pPlayer->destRect, m_pBosses[i]->destRect)))
				{
					if (battle(m_pBosses[i]))
					{
						delete m_pBosses[i];
						m_pBosses[i] = nullptr;
						m_bossCount--;
						m_bossKillCount++;
					}
				}
				if (i < mk_maxNumOfFloorObjects && m_pFloorObjects[i] != nullptr && m_pPlayer->getZpos() == m_pFloorObjects[i]->getZpos() &&
					(m_pPlayer->getZpos() == m_pFloorObjects[i]->getZpos() && checkCollision(m_pPlayer->destRect, m_pFloorObjects[i]->destRect)))
				{
					if (battle(m_pFloorObjects[i]))
					{
						delete m_pFloorObjects[i];
						m_pFloorObjects[i] = nullptr;
						m_flrObjCount--;
						m_flrObjKillCount++;
					}
				}
				if (i < mk_maxNumbOfRats && m_pRats[i] != nullptr && m_pPlayer->getZpos() == m_pRats[i]->getZpos() &&
					(m_pPlayer->getZpos() == m_pRats[i]->getZpos() && checkCollision(m_pPlayer->destRect, m_pRats[i]->destRect)))
				{
					if (battle(m_pRats[i]))
					{
						delete m_pRats[i];
						m_pRats[i] = nullptr;
					}
				}
			}
		}
	}
}

void GameManager::ratsUpdate()
{
	whosTurn = RatsTurn;
	for (int i = 0; i < mk_maxNumbOfRats; i++)
	{ //iter through enemies
		if (m_pRats[i] != nullptr)
		{			//if it returns 0 then I know the enemy is not attacking
			m_pRats[i]->move(m_pPlayer, gZone, this);
			m_pRats[i]->update(gZone);
		}
	}
}

//getting and updating the skelly moves
void GameManager::skellyUpdate()
{
	whosTurn = SkelliesTurn;
	for (int i = 0; i < mk_maxNumOfSkellies; i++)
	{ //iter through enemies
		if (m_pSkellies[i] != nullptr)
		{			//if it returns 0 then I know the enemy is not attacking
			m_pSkellies[i]->move(m_pPlayer, gZone, this);
			m_pSkellies[i]->update(gZone);
		}
	}
}
//getting and updating the mummy moves
void GameManager::mummyUpdate()
{
	whosTurn = MummiesTurn;
	for (int i = 0; i < mk_maxNumOfMummies; i++)
	{ //iter through enemies
		if (m_pMummies[i] != nullptr)
		{			//if it returns 0 then I know the enemy is not attacking
			m_pMummies[i]->move(m_pPlayer, gZone, this);
			m_pMummies[i]->update(gZone);
		}
	}
}
//getting and updating the Zombie moves
void GameManager::zombieUpdate()
{
	whosTurn = ZombiesTurn;
	for (int i = 0; i < mk_maxNumOfZombies; i++)
	{ //iter through enemies
		if (m_pZombies[i] != nullptr)
		{			//if it returns 0 then I know the enemy is not attacking
			m_pZombies[i]->move(m_pPlayer, gZone, this);
			m_pZombies[i]->update(gZone);
		}
	}
}
//getting and updating the Boss moves
void GameManager::bossUpdate()
{
	whosTurn = BossesTurn;
	for (int i = 0; i < mk_maxNumOfBosses; i++)
	{ //iter through enemies
		if (m_pBosses[i] != nullptr)
		{			//if it returns 0 then I know the enemy is not attacking
			m_pBosses[i]->move(m_pPlayer, gZone, this);
			m_pBosses[i]->update(gZone);
		}
	}
}
void GameManager::floorObjectsUpdate()
{
	whosTurn = FlrObjTurn;
	for (int i = 0; i < mk_maxNumOfFloorObjects; i++)
	{ //iter through enemies
		if (m_pFloorObjects[i] != nullptr)
		{			//if it returns 0 then I know the enemy is not attacking
			m_pFloorObjects[i]->update(gZone);
		}
	}
}



//if the entity is moving onto the other entity it decreases that entity's health
bool GameManager::battle(Entity *enemy)
{
	isEnemyDead = false;
	if (whosTurn == PlayersTurn)
	{
		enemy->setHealth(-m_pPlayer->getPlayerDmg());
	}
	else
	{
		m_pPlayer->setHealth(-enemy->getDmg());
	}
	

	/////////////Thinking about changing damage output of enemies///////////////////
	if (enemy->getHealth() <= 0.0f)
	{
		updatePlayerStats(m_pPlayer, enemy);

		printf("\033[%d;%dH", 7, 1);
		//can't delete the enemy here because this enemy is still needed to finish the loop.
		isEnemyDead = true;
	}
	if (m_pPlayer->getHealth() <= 0)
	{
		if (m_pPlayer->getMoney() > 250)
			m_pPlayer->setMoney(-250);
		else
			m_pPlayer->setMoney(-m_pPlayer->getMoney());

		m_pPlayer->incrementDeathCount();
		std::cout << "\tYou were knocked out!\n"
			"You were found about a day later and brought to the healer.\n"
			"You paid the healer 250 money to heal your wounds.\n";
		m_pPlayer->setHealth(m_pPlayer->getMaxHealth());
		m_pPlayer->setEndurance(m_pPlayer->getMaxEndurance() - m_pPlayer->getEndurance());
		m_pPlayer->setPosY(-m_pPlayer->getPosY() + m_healerPosY);//this is where the Healer is located
		m_pPlayer->setPosX(-m_pPlayer->getPosX() + m_healerPosX);
		m_pPlayer->setZpos(-m_pPlayer->getZpos());
		std::cin.ignore(256, '\n');

		enemy->setHealth(enemy->getMaxHealth() - enemy->getHealth()); //the enemy that killed you gets healed.
		createEnemies();
		
		system("cls||clear");
	}
	return isEnemyDead;
}

//updates the players stats
void GameManager::updatePlayerStats(Player *player, Entity *enemy)
{
	float money = 0.0f;
	float exp = 0.0f;

	player->incrementKillCount();
	player->setMoney(enemy->getMoney()); //the player picks up the enemies gold
	money = enemy->getMoney();

	if (player->getEndurance() >= enemy->getBonusHeal())
	{
		player->setHealth(enemy->getBonusHeal()); //player heals a little bit after each kill
		player->setEndurance(-enemy->getBonusHeal());
	}
	else
	{//if the player doesn't have enough endurance, then the player only heals for the amount of endurance left
		player->setHealth(player->getEndurance()); //player heals a little bit after each kill
		player->setEndurance(-player->getEndurance());
	}

	//this is just to check to see if the player will level up, if he doed, then I increase the max health and endurance.
	if ((int)(player->getLevel() + (enemy->giveBaseXP * enemy->getLevel()) / (int)player->getLevel())
		== (int)player->getLevel() + 1)
	{
		exp = ((enemy->giveBaseXP * enemy->getLevel()) / (int)player->getLevel());
		player->setLevel(((enemy->giveBaseXP * enemy->getLevel()) / (int)player->getLevel()));
		player->increaseMaxHealth();
		player->increaseMaxEndurance();
		player->setHpotHealAmount();
		player->setEpotHealAmount();
	}
	else
	{
		exp = ((enemy->giveBaseXP * enemy->getLevel()) / (int)player->getLevel());
		player->setLevel(((enemy->giveBaseXP * enemy->getLevel()) / (int)player->getLevel()));
	}
	gainedChat("Exp",exp,"Money", money, true);
}


//welcoming story
void GameManager::printWelcome(Player* player)
{
	
		system("cls||clear");
		std::cout <<
			"----------------------------------------------------------------------- \n"
			"If the line above is not just one straight line, please resize your \n"
			"window to fit the line.\n\n\n\n";
		std::cout <<
			"Press Enter to continue after you have resized your window...";
		std::cin.ignore(256, '\n');


		if (!player->getMayorFlag())
		{
		system("cls||clear");
		std::cout <<
			"----------------------------------------------------------------------- \n"
			"     Traveling, thinking of nothing but what had happened to your \n"
			"family. You can't stop thinking about what else you could have done \n"
			" to protect your family. How could you have left that one night. Your \n"
			"wife had even asked you to stay with her and the kids and the only \n"
			"thing you could say was...\n\n\n\n"
			"Press Enter to continue...";
		std::cin.get();
		system("cls||clear");
		std::cout <<
			"----------------------------------------------------------------------- \n"
			"\t'I must go! I have to provide food for everyone. If I stayed \n"
			"\tthis night, I will miss the pack of buffalo! Good Bye!'.\n\n\n\n"
			"Press Enter to continue...";
		std::cin.get();
		system("cls||clear");
		std::cout <<
			"----------------------------------------------------------------------- \n"
			"     Of course you didn't know what was going to happen, there was no \n"
			" way you could have. But knowing that your last words were not 'I \n"
			"love you' is what hurts the most. It has been months now since the \n"
			"death of your family and you have been doing nothing but following \n"
			"the mist that you believe caused this to happen.\n\n\n\n"
			"Press Enter to continue...";
		std::cin.get();
		system("cls||clear");
		std::cout <<
			"----------------------------------------------------------------------- \n"
			"     You are close to entering a small town by the name of 'Xael' to \n"
			"find answers. You almost wish you hadn't come this far. But now that \n"
			"you are here, there is no turning back. You should go now, you must \n"
			"speak with the 'M'ayor.\n\n\n\n"
			"Press Enter to continue...";
		std::cin.get();
	}
	system("cls||clear");
	std::cout <<
		"----------------------------------------------------------------------- \n"
		"\t\tGAME CONTROLLS" << std::endl <<

		"\t\t\tP = Pause(use potions from here)" << std::endl <<
		"\tW = Up"					"\t\t\t\tI = Attack Up" << std::endl <<
		"A = Left"	"\tD = Right    J = Attack Left"		"\tL = Attack Right" << std::endl <<
		"\tS = Down"				"\t\t\tK = Attack Down" << std::endl << std::endl <<

		"\t\tMAP LEGEND" << std::endl <<
		"'M' = Mayor\t'H' = Healer\t '$' = Shop" << std::endl <<
		"'v' = Stairs Down\t'^' = Stairs Up\n"
		"'S' = Skeleton\t'&' = Mummy\t'Z' = Zombie\t'B' = Boss\n\n"
		"Tip: In order to talk to the people in town, just walk into them.\n"
		"Tip: Attack faster by holding down the attack key.\n\n\n"
		"Press Enter to continue...";
	std::cin.get();
	system("cls||clear");
}

//speak to the mayor
void GameManager::mayorSpeach(Player* player)
{
	system("cls||clear");
	std::cout <<
		"----------------------------------------------------------------------- \n"
		"     Welcome, " << player->getName() << "! Have you come to rid us of \n"
		"our 'Deadless'? You haven't heard? Well please, let me explain myself \n"
		"then. About full season ago, a mysterious dark mist had passed \n"
		"through our small town of 'Xael'. A few days after the passing we \n"
		"started to see our ancestors sarcophagi open and out came their bones. \n"
		"They were very hostile and started terrorizing my people.\n\n\n\n"
		"Press Enter to continue...";
	std::cin.get();
	system("cls||clear");
	std::cout <<
		"----------------------------------------------------------------------- \n"
		"     We had another traveler come by here shortly after the mist fell\n"
		"upon our city. He decided to stay here until we were able to conquer\n"
		"what has been haunting the town. We have not had much success. I find\n"
		"myself at a disadvantage. Will you stay and help with this mess. \n"
		"Between you and the healer, I feel you will be able to find the source \n"
		"of the problem and free us from this evil.\n\n\n\n"
		"Press Enter to continue...";
	std::cin.get();
	system("cls||clear");
	std::cout <<
		"----------------------------------------------------------------------- \n"
		"     " << player->getName() << ", please help us and kill the things that lurk in \n"
		"our catacombs. If you need your wounds looked after, please see the \n"
		"Healer. You may also buy potions at the Shop.\n\n\n\n"
		"Press Enter to continue...";
	std::cin.get();
	system("cls||clear");

}

void GameManager::dungeonDoor(Player* player, bool& mayerFlag)
{
	if (!mayerFlag)
	{
		std::cout << player->getName() << ": I should go speak to the Mayor before I enter their catacombs!\n";
		std::cin.ignore(256, '\n');
	}
}

//pause menu, able to use potions from here
void GameManager::pauseGame(Player *player, GameManager *GM) {
	int choice = -1;
	while (choice != 0 && choice !=9)
	{
		system("cls || clear");
		std::cout << "\n\n\n\t\tGAME PAUSED\n\n\n\n"
			"\tHealth: " << player->getHealth() << "/" << player->getMaxHealth() <<
			"\tEndurance: " << player->getEndurance() << "/" << player->getMaxEndurance() <<
			"\n\n\tWould you like to use a potion?\n\n"
			"1: \tHealth Potion -- Own: " << player->getHealthPots() << "\n"
			"2: \tEndurance Potion -- Own: " << player->getEndurancePots() << "\n"
			"3: \tSpell Town Teleport -- Own: " << player->getTeleportSpells() << "\n"
			"4: \tSave Game\t'Name of save is the name of your Character.\n"
			"0: \tExit Menu\n\n\n"
			"9: END GAME\n\n";
		std::cin >> choice;
		std::cin.ignore();
		while (std::cin.fail())
		{
			std::cout << "Error" << std::endl;
			std::cin.clear();
			std::cin.ignore(256, '\n');
			std::cin >> choice;
		}

		std::string loading = "|            |";
		switch (choice)
		{
		case 1:

			if (player->getHealthPots() < 1)
			{
				std::cout << "\nI'm sorry, you don't have any health potions.\n\n";

				std::cout << "\n\n\n\nPress Any Key to Continue...\n";
				_getch();
			}
			else if (player->getHealth() >= player->getMaxHealth() && player->getHealthPots() > 0)
			{
				std::cout << "You raise a jar, and just before it hits your lips, you realized you \n"
					"were about to waste a perfectly good potion. \n"
					"\tYou are already at full health...";
				std::cout << "\n\n\n\nPress Any Key to Continue...\n";
				_getch();
			}
			else
			{
				std::cout << "\nYou were healed for " << player->getHpotHealAmount() - player->getHealth() << "!";
				player->setHealth(player->getHpotHealAmount() - player->getHealth());
				player->setHealthPots(-1);
				player->incrementHealthPotsUsed();

				std::cout << "\n\n\n\nPress Any Key to Continue...\n";
				_getch();
			}
			break;
		case 2:
			if (player->getEndurance() >= player->getMaxEndurance() && player->getEndurancePots() > 0)
			{
				std::cout << "You raise a jar, and just before it hits your lips, you realized you \n"
					"were about to waste a perfectly good potion. \n"
					"\tYou are have max enduance...";
				std::cout << "\n\n\n\nPress Any Key to Continue...\n";
			}
			else if (player->getEndurancePots() > 0)
			{
				std::cout << "\nYour Endurance has recovered by " << player->getEpotHealAmount() - player->getEndurance();
				player->setEndurance(player->getEpotHealAmount() - player->getEndurance());

				player->setEndurancePots(-1);
				player->incrementEndurPotsUsed();

				std::cout << "\n\n\n\nPress Any Key to Continue...\n";
			}
			else
			{
				std::cout << "\nI'm sorry, you don't have any endurance potions.\n\n";

				std::cout << "\n\n\n\nPress Any Key to Continue...\n";
			}
			break;
		case 3:
			if (player->getTeleportSpells() < 1)
			{
				std::cout << "\nI'm sorry, you don't have any Teleport Spells, You should go buy one.\n\n";

				std::cout << "\n\n\n\nPress Any Key to Continue...\n";
			}
			else if (player->getZpos() == 0)
			{
				
				std::cout << "You realize you are already in town and it would be a wasted spell!\n";
			}
			else
			{
				player->setTpBackPosXY();//sets both x and y in function
				player->setTpBackZpos(player->getZpos());
				GM->createPortals();
				GM->portalActive = true;

				

				std::cout << "\nYou are returning to town! Using a portal while 'IN' town will allow you to return here!";
				player->setZpos(-player->getZpos());
				player->setPosY(-player->getPosY() + GM->portalInTownPosY);
				player->setPosX(-player->getPosX() + GM->portalInTownPosX);
				player->setTeleportSpells(-1);
				player->incrementTeleportSpellsUsed();

				std::cout << "\n\n\n\nPress Any Key to Continue...\n";
				choice = 0;
			}
			break;
		case 4:
			
			player->save();
			
			using namespace std::chrono_literals;
			using namespace std::this_thread;
			
			std::cout << "\n\n\t\t..Now Saving..\n";
			std::cout << "\t\t" << loading;
			loading.replace(1,1, "X");
			sleep_for(70ms);
			std::cout << "\r\t\t" << loading;
			loading.replace(2, 1, "X");
			sleep_for(70ms);
			std::cout << "\r\t\t" << loading;
			loading.replace(3, 1, "X");
			sleep_for(70ms);
			std::cout << "\r\t\t" << loading;
			loading.replace(4, 1, "X");
			sleep_for(70ms);
			std::cout << "\r\t\t" << loading;
			loading.replace(5, 1, "X");
			sleep_for(70ms);
			std::cout << "\r\t\t" << loading;
			loading.replace(6, 1, "X");
			sleep_for(70ms);
			std::cout << "\r\t\t" << loading;
			loading.replace(7, 1, "X");
			sleep_for(70ms);
			std::cout << "\r\t\t" << loading;
			loading.replace(8, 1, "X");
			sleep_for(70ms);
			std::cout << "\r\t\t" << loading;
			loading.replace(9, 1, "X");
			sleep_for(70ms);
			std::cout << "\r\t\t" << loading;
			loading.replace(10, 1, "X");
			sleep_for(70ms);
			std::cout << "\r\t\t" << loading;
			loading.replace(11, 1, "X");
			sleep_for(70ms);
			std::cout << "\r\t\t" << loading;
			loading.replace(12, 1, "X");
			sleep_for(70ms);
			std::cout << "\r\t\t" << loading;
			sleep_for(40ms);
			std::cout << "\n\nFile Saved as " << player->getName() << ".txt";
			break;
		case 9:
			GM->isRunning = false;

			break;
		default:
			break;
		}
		_getch();
		system("cls||clear");
	}
}

//print the shop and buy potions
void GameManager::shopping(Player *player)
{
	int choice = -1;
	while (choice != 0)
	{
		system("cls || clear");
		std::cout << "Welcome, this is the potions shop.\n"
			"What can I get for you?\n\n\n\n"
			"1: \tHealth Potion    \tcost: " << mk_healthPotPrice << "\n"
			"2: \tEndurance Potion \tcost: " << mk_endurancePotPrice << "\n"
			"3: \tSpell Return Home\tcost: " << mk_teleportTownPrice << "\n"
			"4: \tWeapon Upgrade   \tcost: " << mk_weaponUpgradePrice * player->getPlayerDmg() << "\n"
			"0: Exit\n\n\n"
			"Money: " << player->getMoney() << "\n";
		std::cin >> choice;
		std::cin.ignore();
		while (std::cin.fail()) //if the cin captures anything other than what the cin is looking for, it will fail.
		{
			std::cout << "I'm sorry, is that gibberish your speaking?" << std::endl;
			std::cin.clear();
			std::cin.ignore(256, '\n');
			std::cin >> choice;
		}
		switch (choice)
		{
		case 1:
			if (player->getHealthPots() >= player->mk_potsAllowed)
			{
				std::cout << "\nI'm sorry, you can only carry " << player->mk_potsAllowed << " of those at a time.\n";
			}
			else if (player->getMoney() < mk_healthPotPrice)
			{
				std::cout << "\nUh oh, you don't have enough money. Come again another time.\n";
			}
			else
			{
				player->setHealthPots(1);
				player->setMoney(-mk_healthPotPrice);
			}
			break;
		case 2:
			if (player->getEndurancePots() >= player->mk_potsAllowed)
			{
				std::cout << "\nI'm sorry, you can only carry " << player->mk_potsAllowed << " of those at a time.\n";
			}
			else if (player->getMoney() < mk_endurancePotPrice)
			{
				std::cout << "\nUh oh, you don't have enough money. Come again another time.\n";
			}
			else
			{
				player->setEndurancePots(1);
				player->setMoney(-mk_endurancePotPrice);
			}
			break;
		case 3:
			if (player->getMoney() < mk_teleportTownPrice)
			{
				std::cout << "\nUh oh, you don't have enough money. Come again another time.\n";
			}
			else
			{
				player->setTeleportSpells(1);
				player->setMoney(-mk_teleportTownPrice);
			}
			break;
		case 4:
			if (player->getMoney() < mk_weaponUpgradePrice * player->getPlayerDmg())
			{
				std::cout << "\nUh oh, you don't have enough money. Come again another time.\n";
			}
			else
			{
				std::cout << "\n\tYour damage was " << player->getPlayerDmg();
				player->setMoney(-(mk_weaponUpgradePrice * player->getPlayerDmg()));
				player->setPlayerDmg(0.25);
				std::cout << "\n\tYour damage is now " << player->getPlayerDmg();
			}
			break;
		default:
			std::cout << "\n\nAlright, I will see you again some time. Please be safe!\n";
			break;
		}
		std::cout << "\n\n\nPress Any Key to Continue...\n";
		_getch();
	}
}



//creating the enemy
void GameManager::createSkellies()
{
	whosTurn = SkelliesTurn;
	int zPos = 0;
	for (int i = 0; i < mk_maxNumOfSkellies; i++)
	{//rand position each time create is called so even if not nullptr will relocate those enemies..
		randyPos = rand();
		randxPos = rand();
		if (m_pSkellies[i] == nullptr)
		{
			do
			{
				zPos = randZlevel();
				randyPos = randPosition(gZone->MapHEIGHTSy[zPos]);
				randxPos = randPosition(gZone->MapWIDTHSx[zPos]);
			} while (gZone->getTile(zPos, randyPos, randxPos) != 1);

			int money = (int)rand() % 30 + 10;
			m_pSkellies[i] = new Skeleton(*winRenderer, spriteFileS, m_sName, m_skellySymbol, m_skellyLevel, m_skellyHealth, m_skellyEndurance, money, randyPos, randxPos, zPos, m_skellyBonusHeal);
			m_skellyCount++;
			
		}
	}
	for (int i = 0; i < mk_maxNumbOfRats; i++)
	{
		randyPos = rand();
		randxPos = rand();
		if (m_pRats[i] == nullptr)
		{
			int money = 2;
			do
			{
				zPos = randZlevel();
				randyPos = randPosition(gZone->MapHEIGHTSy[zPos]);
				randxPos = randPosition(gZone->MapWIDTHSx[zPos]);
			} while (gZone->getTile(zPos, randyPos, randxPos) != 1);
			m_pRats[i] = new Skeleton(*winRenderer, spriteRATS, m_ratsName, m_ratsSymbol, m_ratsLevel, m_ratsHealth, m_NPCEndurance, money, randyPos, randxPos, zPos, m_ratsBonusHeal);
		}
	}
}
void GameManager::createMummies()
{
	whosTurn = MummiesTurn;
	
	int zPos = 0;
	for (int i = 0; i < mk_maxNumOfMummies; i++)
	{
		randyPos = rand();
		randxPos = rand();
		if (m_pMummies[i] == nullptr)
		{
		do
		{
			zPos = randZlevel();
			randyPos = randPosition(gZone->MapHEIGHTSy[zPos]);
			randxPos = randPosition(gZone->MapWIDTHSx[zPos]);
		} while (gZone->getTile(zPos, randyPos, randxPos) != 1);
			int money = (int)rand() % 50 + 30;
			m_pMummies[i] = new Mummy(*winRenderer, spriteFileM, m_mName, m_mummySymbol, m_mummyLevel, m_mummyHealth, m_mummyEndurance, money, randyPos, randxPos, zPos, m_mummyBonusHeal);
			m_mummyCount++;
		}
		
	}
}
void GameManager::createZombies()
{
	whosTurn = ZombiesTurn;
	
	int zPos = 0;
	for (int i = 0; i < mk_maxNumOfZombies; i++)
	{
		randyPos = rand();
		randxPos = rand();
		if (m_pZombies[i] == nullptr)
		{
		do
		{
			zPos = randZlevel();
			randyPos = randPosition(gZone->MapHEIGHTSy[zPos]);
			randxPos = randPosition(gZone->MapWIDTHSx[zPos]);
		} while (gZone->getTile(zPos, randyPos, randxPos) != 1);
			int money = (int)rand() % 75 + 40;
			m_pZombies[i] = new Zombie(*winRenderer, spriteFileZ, m_zName, m_zombieSymbol, m_zombieLevel, m_zombieHealth, m_zombieEndurance, money, randyPos, randxPos, zPos, m_zombieBonusHeal);
			m_zombieCount++;
		}
		
	}
}
void GameManager::createBosses()
{
	whosTurn = BossesTurn;
	
	int zPos = 0;
	for (int i = 0; i < mk_maxNumOfBosses; i++)
	{
		randyPos = rand();
		randxPos = rand();
		if (m_pBosses[i] == nullptr)
		{
		do
		{
			zPos = randZlevel();
			randyPos = randPosition(gZone->MapHEIGHTSy[zPos]);
			randxPos = randPosition(gZone->MapWIDTHSx[zPos]);
		} while (gZone->getTile(zPos, randyPos, randxPos) != 1);
			
			int money = (int)rand() % 1000 + 1001;
			m_pBosses[i] = new Boss(*winRenderer, spriteFileB, m_bossName, m_bossSymbol, m_bossLevel, m_bossHealth, m_bossEndurance, money, randyPos, randxPos, zPos, m_bossBonusHeal);
			m_bossCount++;
		}
	}
}
void GameManager::createFloorObjects()
{
	whosTurn = FlrObjTurn;
	
	int zPos = 0;
	for (int i = 0; i < mk_maxNumOfFloorObjects; i++)
	{
		randyPos = rand();
		randxPos = rand();
		if (m_pFloorObjects[i] == nullptr)
		{
			do
			{
				zPos = randZlevel();
				randyPos = randPosition(gZone->MapHEIGHTSy[zPos]);
				randxPos = randPosition(gZone->MapWIDTHSx[zPos]);
			} while (gZone->getTile(zPos, randyPos, randxPos) != 1);
			int money = (int)rand() % 269 + 46;
			m_pFloorObjects[i] = new FloorObjects(*winRenderer, spriteFileO, m_objName, m_flrObjSymbol, m_flrObjLevel, m_flrObjHealth, m_flrObjEndurance, money, randyPos, randxPos, zPos, m_flrObjBonusHeal);
			m_flrObjCount++;
		}
	}
}
void GameManager::checkWin()
{
	if (m_bossCount < 1)
	{
		system("cls || clear");
		std::cout <<
			"----------------------------------------------------------------------- \n"
			"\n\n\t\t\tWOW!!! Congratulations " << m_pPlayer->getName() << "!\n\n"
			"     You have officially vanquished the source of the mysterious power \n"
			"behind the 'Deadless'... Or so you thought... You traveled back up the \n"
			"stairs. Weak after a long battle with the largest enemy you have ever \n"
			"seen! The healer takes you under his wing to mend you back to health \n"
			"just one last time.";

		std::cout << "\n\n\nPress Enter To Continue...";

		std::cin.ignore(256, '\n');
		system("cls||clear");

		std::cout <<
			"----------------------------------------------------------------------- \n"
			"     But not even a minute has passed by after he healed you and \n"
			"already you begin to hear the screams and howls of the deadless' again! \n"
			"You think to yourself, was it your strength that gives the 'deadless' \n"
			"life? Were you the reason your family died!!! NO! \n";

		std::cout << "\n\n\nPress Enter To Continue...";
		std::cin.get();
		system("cls||clear");

		std::cout <<
			"----------------------------------------------------------------------- \n"
			"     As you come to the realization, you see a very ominous smile come \n"
			"from the healer himself. He was standing over you, knowing that you now \n"
			"knew his secret, HE THRUSTS HIS DAGGER DOWN with all his might in an \n"
			"attempt to KILL YOU! \n"
			"\n(Is this where the hero dies? Perhaps a story for the next time!)";

		std::cout << "\n\n\nPress Enter To Continue...";
		std::cin.ignore(256, '\n');
		system("cls||clear");

		std::cout <<
			"----------------------------------------------------------------------- \n"
			"\nHere are your end game stats...\n\n"
			"\nSkeletons Slain: " << m_skellyKillCount <<
			"\nMummies Slain: " << m_mummyKillCount <<
			"\nZombies Slain: " << m_zombieKillCount <<
			"\nBosses Slain: " << m_bossKillCount <<
			"\n\nTotal Deadless Slain: " << m_pPlayer->getKillCount() <<
			"\n\nTotal times you died: " << m_pPlayer->getDeathCount() <<
			"\n\nTimes walked to the Healer to heal: " << m_pPlayer->gettimesHealer() <<
			"\nHealth pots used: " << m_pPlayer->getHealthPotsUsed() <<
			"\nEndurance pots used: " << m_pPlayer->getEndurPotsUsed() <<
			"\n\n\nPress Enter To Exit...";
		std::cin.ignore(256, '\n');
		std::cout << "\n\n\nPlease, continue playing the game! Get stronger and stronger,\n"
			"your goal is to kill the boss as many times as you can, earn enough gold to\n"
			"to buy as many weapon upgrades as possible and kill the boss in one shot!\n";

		std::cin.ignore(256, '\n');
		isRunning = true;
		createEnemies();
		std::cout << "\nYou are returning to town!";
		m_pPlayer->setZpos(-m_pPlayer->getZpos());
		m_pPlayer->setPosX(-m_pPlayer->getPosX() + m_healerPosX);
		m_pPlayer->setPosY(-m_pPlayer->getPosY() + m_healerPosY);
		m_pPlayer->incrementTeleportSpellsUsed();
	}
}

//randomize a potion
float GameManager::randPosition(int maxPos)
{
	float randnumber; 
	do
	{
		for (int i = 0; i < 9; i++)
			randnumber = (int)rand() + 1;

	} while (randnumber > maxPos - 2 );
	return (int)randnumber;
}


//randomize Z position
int GameManager::randZlevel()
{   // Skellies are week and are first encounter.
	int possibleZ = gZone->currentNumberOfZ;
	int tmpZ = 0;
	int counter = 15;
	do
	{
		if (possibleZ > 1)
		{


			if (whosTurn == SkelliesTurn)
				tmpZ = (int)(rand() % possibleZ + 1);
			else if (whosTurn == MummiesTurn)
				tmpZ = (int)(rand() % possibleZ + 2);
			else if (whosTurn == ZombiesTurn)
				tmpZ = (int)(rand() % possibleZ + 3);
			else if (whosTurn == FlrObjTurn)
				tmpZ = (int)(rand() % possibleZ + 3);	
		}
		counter--;
		if (counter < 1)
			tmpZ = possibleZ - 1;

	} while (tmpZ > possibleZ - 1);
	if (whosTurn == BossesTurn)
		return possibleZ - 1;


	return tmpZ;
}
