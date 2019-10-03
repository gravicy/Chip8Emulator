#pragma once

#include <array>

namespace SDLWrapper
{
	namespace Display
	{
		bool init(const int width, const int height);
		void render(const std::array<std::array<bool, 32>, 64>& pixelMap);
		void quit();
	}
}