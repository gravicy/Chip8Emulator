#pragma once

#include <array>

namespace SDLWrapper
{
	namespace Display
	{
		bool init(const int width, const int height);
		void render(const std::array<bool, 64 * 32>& pixelMap);
		void quit();
	}
}