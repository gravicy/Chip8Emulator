#include "Input.h"

#include <SDL.h>
#include <algorithm>

namespace SDLWrapper
{
	namespace Input
	{
		namespace
		{
			std::array<uint8_t, 16> keyMap{
				SDLK_1,
				SDLK_2,
				SDLK_3,
				SDLK_4,
				SDLK_q,
				SDLK_w,
				SDLK_e,
				SDLK_r,
				SDLK_a,
				SDLK_s,
				SDLK_d,
				SDLK_f,
				SDLK_y,
				SDLK_x,
				SDLK_c,
				SDLK_v
			};
		}

		bool init()
		{
			if (SDL_Init(SDL_INIT_VIDEO) < 0)
				return false;

			return true;
		}

		bool setKeys(std::array<bool, 16>& keys)
		{
			SDL_Event sdlEvent;
			while (SDL_PollEvent(&sdlEvent))
			{
				switch (sdlEvent.type)
				{
				case SDL_QUIT:
					return false;

				case SDL_KEYDOWN:
					if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
						return false;

					keys[std::distance(keyMap.begin(),
						std::find(keyMap.begin(), keyMap.end(), sdlEvent.key.keysym.sym))] = 1;
					break;

				case SDL_KEYUP:

					keys[std::distance(keyMap.begin(),
						std::find(keyMap.begin(), keyMap.end(), sdlEvent.key.keysym.sym))] = 0;
					break;

				default:
					break;
				}
			}

			return true;
		}
	}
}