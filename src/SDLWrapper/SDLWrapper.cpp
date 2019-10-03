#include "SDLWrapper.h"
#include "Display.h"
#include "Input.h"

#include <SDL.h>

namespace SDLWrapper
{
	bool init(const int width, const int height)
	{
		return (Display::init(width, height) && Input::init());
	}

	void render(const std::array<std::array<bool, 32>, 64>& pixelMap)
	{
		Display::render(pixelMap);
	}

	bool checkInput(std::array<bool, 16>& keys)
	{
		if (!Input::setKeys(keys))
		{
			quit();
			return false;
		}

		return true;
	}

	void quit()
	{
		Display::quit();
		SDL_Quit();
	}
}