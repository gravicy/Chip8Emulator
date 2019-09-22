#include "Chip8.h"
#include <fstream>
#include <iostream>


Chip8::Chip8()
	: opcode(0), memory(), vRegister(), indexRegister(0)
	, programCounter(0x0u), pixelMap(), delayTimer(0), soundTimer(0)
	, stack(), keys(), drawFlag(false), opcodeMap()
{
	opcodeMap.emplace(0x1u, [this]()
	{
		programCounter = opcode & 0x0FFF;
	});
	opcodeMap.emplace(0xAu, [this]()
	{
		indexRegister = opcode & 0x0FFF;
		programCounter += 2;
	});
}

void Chip8::loadGame(std::string filename)
{
	std::ifstream file;
	file.open(filename, std::ios::binary | std::ios::ate);

	if (!file.fail() && file.good())
	{
		auto size = file.tellg();
		file.seekg(0, file.beg);
		file.read((char*)&memory[programCounter], size);

		file.get();
	}
	else
	{
		std::cerr << "Failed to open file: " << filename << std::endl;
	}
}

void Chip8::emulateCycle()
{
	opcode = memory[programCounter] << 8 | memory[programCounter + 1];

	auto mapKey = (opcode & 0xF000) >> 12;

	// if there is no valid opcode, just increase the program counter
	if (opcodeMap.find(mapKey) == opcodeMap.end())
		programCounter += 2;
	else
		opcodeMap.at(mapKey)();
}
