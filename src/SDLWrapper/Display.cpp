#include "Display.h"

#include <SDL.h>

namespace SDLWrapper
{
	namespace Display
	{
		namespace
		{
			SDL_Window* window;
			SDL_Renderer* renderer;
		}

		bool init(const int width, const int height)
		{
			if (SDL_Init(SDL_INIT_VIDEO) < 0)
				return false;

			// Create a window, we don't care about position
			window = SDL_CreateWindow(
				"CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				width, height, SDL_WINDOW_SHOWN);
			if (window == nullptr)
				return false;

			// Create renderer
			renderer = SDL_CreateRenderer(window, -1, 0);
			SDL_RenderSetLogicalSize(renderer, 64, 32);

			return true;
		}

		void render(const std::array<bool, 64*32>& pixelMap)
		{
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(renderer);

			for (unsigned int x = 0; x < 64; x++)
			{
				for (unsigned int y = 0; y < 32; y++)
				{
					if (pixelMap[x + y * 32])
					{
						SDL_Rect fillRect = { x, y, 1u, 1u };
						SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
						SDL_RenderFillRect(renderer, &fillRect);
					}
				}
			}

			SDL_RenderPresent(renderer);

			// TODO remove
			SDL_Delay(5000);
		}

		void quit()
		{
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
		}
	}
}