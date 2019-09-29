#pragma once

#include <array>

namespace SDLWrapper
{
	bool init(const int width, const int height);
	void renderAndCheckInput(const std::array<bool, 64 * 32>& pixelMap, const std::array<bool, 16>& keys);
	void quit();
}
