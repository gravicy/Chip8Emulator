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
	uint16_t					opcode;
	std::array<uint8_t, 4096>	memory;

	// Registers V0 to VE and VF-flag
	std::array<uint8_t, 16>		vRegister;

	uint16_t					indexRegister;
	uint16_t					programCounter;
	std::array<bool, 64*32>		pixelMap;
	uint16_t					delayTimer;
	uint16_t					soundTimer;
	std::stack<uint16_t>		stack;
	std::array<bool, 16>		keys;
	bool						drawFlag;

	typedef std::function<void()> Operation;

	// for calling the correct function to the corresponding opcode
	std::unordered_map<uint16_t, Operation> opcodeMap;
	std::unordered_map<uint16_t, Operation> opcodeMap0;
	std::unordered_map<uint16_t, Operation> opcodeMap8;
	std::unordered_map<uint16_t, Operation> opcodeMapE;
	std::unordered_map<uint16_t, Operation> opcodeMapF;

public:
	Chip8();

	void loadGame(std::string filename);

	void emulateCycle();

	bool getDrawFlag();

	const std::array<bool, 64 * 32>& getPixelMap();

	void setKeys(std::array<bool, 16>& keys);
};
