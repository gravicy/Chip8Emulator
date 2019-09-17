#include "Display.h"

bool SDLWrapper::Display::init(const int width, const int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return false;

	// Create a window, we don't care about position
	SDLWrapper::Display::window = SDL_CreateWindow(
		"CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_SHOWN);
	if (window == nullptr)
		return false;

	// Create renderer
	SDLWrapper::Display::renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetLogicalSize(renderer, 64, 32);

	return true;
}

void SDLWrapper::Display::render(std::array<std::array<bool, 32>, 64>& pixelMap)
{
	SDL_SetRenderDrawColor(SDLWrapper::Display::renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(SDLWrapper::Display::renderer);

	for (unsigned int x = 0; x < pixelMap.size(); x++)
	{
		for (unsigned int y = 0; y < pixelMap[x].size(); y++)
		{
			if (pixelMap[x][y])
			{
				SDL_Rect fillRect = { x, y, 1, 1 };
				SDL_SetRenderDrawColor(SDLWrapper::Display::renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderFillRect(SDLWrapper::Display::renderer, &fillRect);
			}
		}
	}

	SDL_RenderPresent(SDLWrapper::Display::renderer);

	// TODO remove
	SDL_Delay(5000);
}

void SDLWrapper::Display::quit()
{
	SDL_DestroyRenderer(SDLWrapper::Display::renderer);
	SDL_DestroyWindow(window);
}