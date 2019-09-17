#pragma once

#include <array>

namespace SDLWrapper
{
	bool init(const int width, const int height);
	void renderAndCheckInput(std::array<std::array<bool, 32>, 64>& pixelMap, std::array<bool, 16>& keys);
	void quit();
}
