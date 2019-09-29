#include <iostream>
#include <array>
#include "SDLWrapper/SDLWrapper.h"
#include "Chip8.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 320;

int main(int argc, char* argv[])
{
	SDLWrapper::init(640, 320);

	Chip8 chip8;
	chip8.loadGame("SpaceInvaders.ch8");

	std::array<bool, 16> keys;

	for (bool& key : keys)
		key = false;

	// checkInput returns false if the user quits the application
	while (SDLWrapper::checkInput(keys))
	{
		chip8.emulateCycle();

		if (chip8.getDrawFlag())
			SDLWrapper::render(chip8.getPixelMap());
	}

	SDLWrapper::quit();

	return 0;
}