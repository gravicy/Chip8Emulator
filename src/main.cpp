#include <iostream>
#include <array>
#include <chrono>
#include <thread>

#include "SDLWrapper/SDLWrapper.h"
#include "Chip8.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 320;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "please provide a rom name as an argument.\n";
		return 1;
	}

	SDLWrapper::init(640, 320);

	Chip8 chip8;
	chip8.loadGame(argv[1]);


	// checkInput returns false if the user quits the application
	while (SDLWrapper::checkInput(chip8.getKeys()))
	{
		chip8.emulateCycle();

		if (chip8.getDrawFlag())
			SDLWrapper::render(chip8.getPixelMap());

		std::this_thread::sleep_for(std::chrono::microseconds(1200));
	}

	SDLWrapper::quit();

	return 0;
}