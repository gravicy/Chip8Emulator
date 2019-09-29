#pragma once

#include <array>

namespace SDLWrapper
{
	bool init(const int width, const int height);
	void render(const std::array<bool, 64 * 32>& pixelMap);
	bool checkInput(std::array<bool, 16>& keys);
	void quit();
}
