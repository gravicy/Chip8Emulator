#pragma once

#include <array>

namespace SDLWrapper
{
	namespace Input
	{
		bool init();

		bool setKeys(std::array<bool, 16>& keys);
	}
}