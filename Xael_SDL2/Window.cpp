#include "Window.h"


Zone *map = nullptr;

Window::Window(int &width, int &height, Zone *maps) :
	_windowWidth(width), _windowHeight(height)
{
	map = maps;

	isRunning = true;
	

}


Window::~Window()
{

}
void Window::init(std::string &mfile)
{
	
	bool fullscreen = false;
	

	
	//map = new Zone(_mapPath[mapIndex]);

	//std::cout << "Would you like to play in fullscreen?\n"
	//	"0: No\n"
	//	"1: Yes\n";

	//std::cin >> fullscreen;

	MapInfo(mfile);

	//int flags = 0;

	//if (fullscreen)
	//{
	//	flags = SDL_WINDOW_FULLSCREEN;
	//}

	//initialize and check success.
	if (!(SDL_Init(SDL_INIT_VIDEO) == 0))
	{
		std::cerr << "Subsystems failed to initialize!" << std::endl;
		isRunning = false;
	}
	else
	{
		isRunning = true;
		//create and check if window created
		if (sdl_pWindow == nullptr)
		{
			if (!(sdl_pWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				_mapTilesWide * _tileWidth, _mapTilesHeight * _tileHeight, SDL_WINDOW_RESIZABLE)))
				///need to replace hard numbers
			{
				std::cerr << "Window creation failed!" << std::endl;
				isRunning = false;
			}
		}

	}
}

void Window::setMapTextures(SDL_Renderer& r)
{
	map->initMapTextures(r);
}


void Window::render(SDL_Renderer* renderer, int &zPos)
{
	//drawing the tiles to the map.
	for (int row = 0; row < map->MapHEIGHTSy[zPos]; row++)
	{
		for (int col = 0; col < map->MapWIDTHSx[zPos]; col++)
		{
			map->drawMap(row, col, renderer, zPos);
		}
	}
}

void Window::handleEvents()
{
	switch (GameManager::events.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN: //any button was pressed down on keyboard
		switch (GameManager::events.key.keysym.sym)
		{
		case SDLK_ESCAPE: //ESC button was pressed
			isRunning = false;
			break;
		default:
			break;
		}
	default:
		break;
	}
}


void Window::clean()
{
	//deletes window
	SDL_DestroyWindow(sdl_pWindow);



	//deletes all subsystems
	SDL_Quit();

	std::cout << "Game Exited!";
}

void Window::MapInfo(std::string& mfile)
{
	std::ifstream readMapFile;

	readMapFile.open(mfile);
	if (readMapFile.fail())
	{
		std::cerr << "Failed to open " + mfile;
	}
	//getting map information
	char nxtMap;
	int mapZ;
	std::string garbage;

	std::getline(readMapFile, garbage);
	readMapFile>> _tileHeight >> _tileWidth;
	readMapFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::getline(readMapFile, garbage);
	std::getline(readMapFile, title);
	readMapFile >> mapZ;
	readMapFile >>_mapTilesHeight;
	readMapFile >>_mapTilesWide;

	readMapFile.close();

	std::cout <<
		"Tiles Wide: " << _mapTilesWide << std::endl <<
		"Tiles High: " << _mapTilesHeight << std::endl <<
		"Width of tiles: " << _tileWidth << std::endl <<
		"Height of tiles: " << _tileHeight << std::endl;

	///Maybe change this to a set number, and have map follow char...
	_windowWidth = _mapTilesWide * _tileWidth;
	_windowHeight = _mapTilesHeight * _tileHeight;

	system("pause");
}