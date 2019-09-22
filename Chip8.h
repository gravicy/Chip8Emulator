#pragma once

#include <cstdint>
#include <array>
#include <stack>

class Chip8
{
private:
	uint16_t					opcode;
	std::array<uint8_t, 4096>	memory;
	std::array<uint8_t, 16>		vRegister;
	uint16_t					indexRegister;
	uint16_t					programCounter;
	std::array<uint8_t, 64*32>	pixelMap;
	uint16_t					delayTimer;
	uint16_t					soundTimer;
	std::stack<uint16_t>		stack;
	std::array<bool, 16>		keys;
	bool						drawFlag;

public:
	Chip8();

	void loadGame(std::string filename);

	void emulateCycle();

	bool getDrawFlag();

	void setKeys(std::array<bool, 16>& keys);

};
