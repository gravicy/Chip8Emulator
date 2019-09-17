#include <iostream>
#include <array>
#include "SDLWrapper/SDLWrapper.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 320;

int main(int argc, char* argv[])
{
	SDLWrapper::init(640, 320);

	std::array<std::array<bool, 32>, 64> pixelMap;

	for (auto& row : pixelMap)
	{
		for (bool& value : row)
		{
			value = false;
		}
	}
	pixelMap[0][0] = true;
	pixelMap[63][0] = true;
	pixelMap[63][31] = true;
	pixelMap[0][31] = true;

	std::array<bool, 16> keys;

	SDLWrapper::renderAndCheckInput(pixelMap, keys);

	SDLWrapper::quit();

	return 0;
}