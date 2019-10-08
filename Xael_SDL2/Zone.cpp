#include "GameManager.h"
#include "Player.h"
#include "Skeleton.h"
#include "Mummy.h"
#include "Zombie.h"
#include "Boss.h"
#include "FloorObjects.h"
#include "NPCs.h"


#include "Zone.h"

class GameManager;


Zone::Zone(std::string &fileName)
//init the entire map.
{
	_tilePath[0] = "assets/map_tiles/Water32x32.png";
	_tilePath[1] = "assets/map_tiles/Dirt32x32.png";
	_tilePath[2] = "assets/map_tiles/Grass_Lisa.png";
	_tilePath[3] = "assets/map_tiles/StonePath32x32.png";
	_tilePath[4] = "assets/map_tiles/brickWall_Lisa.png";
	_tilePath[5] = "assets/map_tiles/brickWallMouse_Lisa.png";
	_tilePath[6] = "assets/map_tiles/upStair.png";
	_tilePath[7] = "assets/map_tiles/downStair.png";
	_tilePath[8] = "assets/map_tiles/Dirt32x32.png";
	


	file.open(fileName);
	if (file.fail())
	{
		perror(fileName.c_str());
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		exit(4);
	}
	else
	{
		const std::string nextMap = "+";
		std::string tmp;
		char tmpCH; //comma collections
		int numberTemp = 0;
		///capture all garbage at top
		std::getline(file, tmp);
		file >> _tileWidth;
		file >> _tileHeight;
		file >> currentNumberOfZ;
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		//create the zlvl array...
		mapsLOADED = new int**[currentNumberOfZ];
		
		for (int z = 0; z < currentNumberOfZ; z++)
		{
			std::getline(file, tmp);
			if (tmp == nextMap)
			{
				std::getline(file, tmp);
				mapNames.push_back(tmp);

				file >> numberTemp;
				MapZPOS.push_back(numberTemp);

				file >> numberTemp;
				MapHEIGHTSy.push_back(numberTemp);

				file >> numberTemp;
				MapWIDTHSx.push_back(numberTemp);

				MapSIZES.push_back(MapWIDTHSx[z] * MapHEIGHTSy[z]);

				file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			else
				std::cout << "failed looping map data.";
			mapsLOADED[z] = new int* [MapHEIGHTSy[z]];
			for (int y = 0; y < MapHEIGHTSy[z]; y++)
			{
				mapsLOADED[z][y] = new int[MapWIDTHSx[z]];
				for (int x = 0; x < MapWIDTHSx[z]; x++)
				{
					file >> numberTemp;
					mapsLOADED[z][y][x] = numberTemp;
					file >> tmpCH;//maps will have comma's that need to be skipped.
				}
				file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}

		}

		file.close();
	}

	src.x = src.y = 0;
	src.w = dest.w = _tileWidth;
	src.h = dest.h = _tileHeight;

	dest.x = dest.y = 0;
}


Zone::~Zone()
{
	SDL_DestroyTexture(water);
	SDL_DestroyTexture(dirt);
	SDL_DestroyTexture(grass);
	SDL_DestroyTexture(stonepath);
	SDL_DestroyTexture(brickWallMouse);
	SDL_DestroyTexture(brickWall);
	SDL_DestroyTexture(upStair);
	SDL_DestroyTexture(downStair);
	SDL_DestroyTexture(lrDoor);
}

//print the map name at top
void Zone::printZoneLevel(int &zPos)
{
	switch (zPos)
	{
	case 0:
		std::cout << "        Xael";
		break;
	default:
		std::cout << "  Dungeon Level: " << zPos;
		break;
	}
}
//initialize the textures for the map.
void Zone::initMapTextures(SDL_Renderer& r)
{
	//due to enum holding int value of CHARs for map. These are manually entered for _tilePath
	water = TextureManager::LoadTexture(r, _tilePath[0]);
	dirt = TextureManager::LoadTexture(r, _tilePath[1]);
	grass = TextureManager::LoadTexture(r, _tilePath[2]);
	stonepath = TextureManager::LoadTexture(r, _tilePath[3]);
	brickWall = TextureManager::LoadTexture(r, _tilePath[4]);
	brickWallMouse = TextureManager::LoadTexture(r, _tilePath[5]);
	upStair = TextureManager::LoadTexture(r, _tilePath[6]);
	downStair = TextureManager::LoadTexture(r, _tilePath[7]);
	lrDoor = TextureManager::LoadTexture(r, _tilePath[8]);
}

//drawing map with textures.
void Zone::drawMap(int &row, int &col, SDL_Renderer* renderer, int &zPos)
{
	
	int type = mapsLOADED[zPos][row][col];

	dest.x = (col * _tileWidth);
	dest.y = (row * _tileHeight);

	switch (type)
	{
	case Water:
		TextureManager::Draw(renderer, water, src, dest);
		break;
	case Grass:
		TextureManager::Draw(renderer, grass, src, dest);
		break;
	case UpStair:
		TextureManager::Draw(renderer, upStair, src, dest);
		break;
	case DownStair:
		TextureManager::Draw(renderer, downStair, src, dest);
		break;
	case Dirt:
		TextureManager::Draw(renderer, dirt, src, dest);
		break;
	case BrickWall:
			TextureManager::Draw(renderer, brickWall, src, dest);
			break;
	case BrickWallMouse:
			TextureManager::Draw(renderer, brickWallMouse, src, dest);
		break;
	case StonePath:
	case LRdoor:
		TextureManager::Draw(renderer, stonepath, src, dest);
		break;
	}

}
//printing the map/zone and adding special characters
void Zone::printZone(Player *player, Skeleton **skelly, Skeleton **rats, Mummy **mummies, Zombie **zombie, Boss **boss, FloorObjects **flrObj, NPCs* Healer, NPCs* Shop, NPCs* Mayor)
{
	/*int zPos = player->getZpos();//this is the zLevel of the player
	printf("\033[%d;%dH", 1, 1);
	
	//system("cls||clear"); //works for windows and Unix systems and I think most OS X machines.
	printZoneLevel(zPos);//print name of the map we are on
	for(int i = 0; i < mapSize[zPos]; i++)
	{
		if (!(i % Width[zPos]))
		{//new line for each row
			std::cout << "\n";
		}
		if (i == player->getPos())
		{
			std::cout << player->getSymbol();
		}
		else if (i == Healer->getPos() && Healer->getZpos() == zPos)
		{
			std::cout << Healer->getSymbol();
		}
		else if (i == Shop->getPos() && Shop->getZpos() == zPos)
		{
			std::cout << Shop->getSymbol();
		}
		else if (i == Mayor->getPos() && Mayor->getZpos() == zPos)
		{
			std::cout << Mayor->getSymbol();
		}
		else if (ActiveMap[zPos][i] == mk_wall || ActiveMap[zPos][i] == '%')
		{
			std::cout << (char)219; // █
		}
		else if (ActiveMap[zPos][i] == mk_water)
		{
			std::cout << (char)247; //  ≈
		}
		else
		{           //Make sure to put the highest enemy count on top!
			for (int j = 0; j < GameManager::mk_maxNumbOfRats; j++)
			{

				if ((rats[j] != nullptr) && rats[j]->getZpos() == zPos && i == rats[j]->getPos())
				{
					std::cout << rats[j]->getSymbol();
					break;
				}
				else if (j < GameManager::mk_maxNumOfSkellies && (skelly[j] != nullptr) && skelly[j]->getZpos() == zPos && i == skelly[j]->getPos())
				{
					std::cout << skelly[j]->getSymbol();
					break;
				}
				else if (j < GameManager::mk_maxNumOfMummies && (mummies[j] != nullptr) && mummies[j]->getZpos() == zPos && i == mummies[j]->getPos())
				{
					std::cout << mummies[j]->getSymbol();
					break;
				}
				else if (j < GameManager::mk_maxNumOfZombies && (zombie[j] != nullptr) && zombie[j]->getZpos() == zPos && i == zombie[j]->getPos())
				{
					std::cout << zombie[j]->getSymbol();
					break;
				}
				else if (j < GameManager::mk_maxNumOfBosses && (boss[j] != nullptr) && boss[j]->getZpos() == zPos && i == boss[j]->getPos())
				{
					std::cout << boss[j]->getSymbol();
					break;
				}
				else if (j < GameManager::mk_maxNumOfFloorObjects && (flrObj[j] != nullptr) && flrObj[j]->getZpos() == zPos && i == flrObj[j]->getPos())
				{
					std::cout << flrObj[j]->getSymbol();
					break;
				}
				else if (j >= GameManager::mk_maxNumbOfRats - 1)
				{
					if (ActiveMap[zPos][i] == '.' || ActiveMap[zPos][i] == '=' || ActiveMap[zPos][i] == ',')
						std::cout << (char)176; // ░
					else
						std::cout << ActiveMap[zPos][i];
				}
			}
		}
	}
	std::cout << "\n\n";
	*/
}