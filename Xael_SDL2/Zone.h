#pragma once
#include "TextureManager.h"
#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include <vector>
#include "../SDL2/include/SDL.h"



class Player;
class NPCs;
class Skeleton;
class Mummy;
class Zombie;
class Boss;
class FloorObjects;

class Zone
{
public://getter/setter
	int getTile(int zPos, float moveY, float moveX) { return mapsLOADED[zPos][static_cast<int>(moveY)][static_cast<int>(moveX)]; }
	void initMapTextures(SDL_Renderer& r);
	void drawMap(int &row, int &col, SDL_Renderer* renderer, int &zPos);

private:

	//char *ZONE = NULL;

	enum Map { Town, Dungeon1, Dungeon2, Dungeon3, Dungeon4, Dungeon5, Dungeon6, Dungeon7, Total_Maps };
	std::vector<std::string> ActiveMap;
	void printZoneLevel(int &zPos);

	std::ifstream file;


	

	

	SDL_Renderer* renderer = nullptr;

	SDL_Rect src, dest;

	SDL_Texture* water;
	SDL_Texture* dirt;
	SDL_Texture* grass;
	SDL_Texture* stonepath;
	SDL_Texture* brickWall;
	SDL_Texture* brickWallMouse;
	SDL_Texture* upStair;
	SDL_Texture* downStair;
	SDL_Texture* lrDoor;

public:
	enum TileType { Water = 0, Dirt, Grass, StonePath, BrickWall, BrickWallMouse,
		UpStair, DownStair, LRdoor, numOfTileTypes};
	std::string _tilePath[numOfTileTypes];
	
	

	Zone(std::string &fileName);
	virtual ~Zone();
	static const char mk_wall = '#';
	static const char mk_downStair = 'v';
	static const char mk_upStair = '^';
	static const char mk_water = '~';
	static const int mk_Start = 12;

	int currentMapWidth = 0;
	int currentMapHeight = 0;
	int currentMapNumber = 0;
	int currentMapSize = 0;
	int currentNumberOfZ = 0;
	int _tileWidth = 32;
	int	_tileHeight = 32;

	std::vector<std::string> mapNames;
	 std::vector<int> MapZPOS;
	 std::vector<int> MapHEIGHTSy;
	 std::vector<int> MapWIDTHSx;
	 std::vector<int> MapSIZES;

	 int*** mapsLOADED = NULL;

	void printZone(Player *player, Skeleton **skelly, Skeleton **rats, Mummy **mummies, Zombie **zombie, Boss **boss, FloorObjects **flrObj, NPCs* Healer, NPCs* Shop, NPCs* Mayor);
};

