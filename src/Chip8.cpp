#include "Chip8.h"
#include <fstream>
#include <iostream>

Chip8::Chip8()
	: opcode(0), memory(), vRegister(), indexRegister(0)
	, programCounter(0), pixelMap(), delayTimer(0), soundTimer(0)
	, stack(), keys(), drawFlag(false)
{
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