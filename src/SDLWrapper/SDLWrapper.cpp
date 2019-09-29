#include "SDLWrapper.h"
#include "Display.h"

namespace SDLWrapper
{
	bool init(const int width, const int height)
	{
		return Display::init(width, height);
	}

	void renderAndCheckInput(const std::array<bool, 64 * 32>& pixelMap, const std::array<bool, 16>& keys)
	{
		Display::render(pixelMap);
	}

	void quit()
	{
		Display::quit();
		SDL_Quit();
	}
}