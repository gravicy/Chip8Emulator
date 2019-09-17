#pragma once

#include <array>
#include <SDL.h>

namespace SDLWrapper
{
	namespace Display
	{
		namespace {
			SDL_Window* window;
			SDL_Renderer* renderer;
		}

		bool init(const int width, const int height);
		void render(std::array<std::array<bool, 32>, 64>& pixelMap);
		void quit();
	}
}