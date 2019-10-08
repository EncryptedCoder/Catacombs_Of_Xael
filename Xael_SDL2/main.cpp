#include "../SDL2/include/SDL.h"
#include "GameManager.h"
#include <random>
#include <time.h>
#include <iostream>
#include "Window.h"

int main(int argc, char* argv[])
{
	unsigned long long seed = time(NULL) * _getpid();

	std::cout <<
		"\n\n\n\n"
		"\t\t\t-----------------------\n"
		"\t\t\t-- CATACOMBS OF XAEL --\n"
		"\t\t\t-----------------------\n"
		"\t\t\tCreated by Jeremy Roath\n\n\n\n\n\n\n"
		"Press enter to continue...";
	std::cin.ignore(256, '\n');
	seed *= time(NULL);
	srand(seed);
	GameManager GM;
	GM.run();

	return 0;
}