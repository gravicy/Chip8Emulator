#include "SDLWrapper.h"
#include "Display.h"

bool SDLWrapper::init(const int width, const int height)
{
	return Display::init(width, height);
}

void SDLWrapper::renderAndCheckInput(std::array<std::array<bool, 32>, 64>& pixelMap, std::array<bool, 16>& keys)
{
	Display::render(pixelMap);
}

void SDLWrapper::quit()
{
	SDLWrapper::Display::quit();
	SDL_Quit();
}