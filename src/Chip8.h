#pragma once

#include <cstdint>
#include <array>
#include <stack>
#include <string>
#include <unordered_map>
#include <functional>

class Chip8
{
private:
	uint16_t								opcode;
	std::array<uint8_t, 4096>				memory;

	// Registers V0 to VE and VF-flag
	std::array<uint8_t, 16>					vRegister;

	uint16_t								indexRegister;
	uint16_t								programCounter;
	std::array<std::array<bool, 32>, 64>	pixelMap;
	uint16_t								delayTimer;
	uint16_t								soundTimer;
	std::stack<uint16_t>					stack;
	std::array<bool, 16>					keys;
	bool									drawFlag;

	typedef std::function<void()> Operation;

	// for calling the correct function to the corresponding opcode
	std::unordered_map<uint16_t, Operation> opcodeMap;
	std::unordered_map<uint16_t, Operation> opcodeMap0;
	std::unordered_map<uint16_t, std::function<void(int, int)>> opcodeMap8;
	std::unordered_map<uint16_t, std::function<void(int)>> opcodeMapE;
	std::unordered_map<uint16_t, std::function<void(int)>> opcodeMapF;

	const uint8_t chip8_fontset[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	void initOpCodeMaps();

public:
	Chip8();

	void loadGame(std::string filename);

	void emulateCycle();

	bool getDrawFlag();

	const std::array<std::array<bool, 32>, 64>& getPixelMap();

	std::array<bool, 16>& getKeys();
};
