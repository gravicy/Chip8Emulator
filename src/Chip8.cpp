#include "Chip8.h"
#include <fstream>
#include <iostream>


Chip8::Chip8()
	: opcode(0), memory(), vRegister(), indexRegister(0)
	, programCounter(0x0u), pixelMap(), delayTimer(0), soundTimer(0)
	, stack(), keys(), drawFlag(false), opcodeMap(), opcodeMap0()
	, opcodeMap8(), opcodeMapE(), opcodeMapF()
{
	for (bool& pixel : pixelMap)
	{
		pixel = false;
	}

	opcodeMap[0x0000u] = [this]() { opcodeMap0[opcode & 0x000F](); };
	opcodeMap[0x1000u] = [this]() { programCounter = opcode & 0x0FFF; };
	opcodeMap[0x2000u] = [this]() { stack.push(programCounter); programCounter = opcode & 0x0FFF; };
	opcodeMap[0x3000u] = [this]()
	{
		if (vRegister[(opcode & 0x0F00) >> 8] == (opcode & 0x0FF))
			programCounter += 4;
		else
			programCounter += 2;
	};
	opcodeMap[0x4000u] = [this]()
	{
		if (vRegister[(opcode & 0x0F00) >> 8] != (opcode & 0x0FF))
			programCounter += 4;
		else
			programCounter += 2;
	};
	opcodeMap[0x5000u] = [this]()
	{
		if (vRegister[(opcode & 0x0F00) >> 8] == vRegister[(opcode & 0x00F0 >> 4)])
			programCounter += 4;
		else
			programCounter += 2;
	};
	opcodeMap[0x6000u] = [this]() { vRegister[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF); programCounter += 2; };
	opcodeMap[0x7000u] = [this]() { vRegister[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF); programCounter += 2; };
	opcodeMap[0x8000u] = [this]() { opcodeMap8[opcode & 0x000F](); };
	opcodeMap[0xA000u] = [this]() { indexRegister = opcode & 0x0FFF; programCounter += 2; };

	//// opcodes starting with 0x0, differentiated by the last 4 bits
	opcodeMap0[0x0000u] = [this]()
	{
		for (bool& pixel : pixelMap)
		{
			pixel = false;
		}
		drawFlag = true;
		programCounter += 2;
	};
	opcodeMap0[0x000Eu] = [this]()
	{
		programCounter = stack.top();
		stack.pop();
		programCounter += 2;
	};
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
	drawFlag = false;

	opcode = memory[programCounter] << 8 | memory[programCounter + 1];

	uint16_t mapKey = opcode & 0xF000;

	// if there is no valid opcode, just increase the program counter until all opcodes are implemented
	if (opcodeMap.find(mapKey) == opcodeMap.end())
		programCounter += 2;
	else
		opcodeMap[mapKey]();
}

bool Chip8::getDrawFlag()
{
	return drawFlag;
}

const std::array<bool, 64 * 32>& Chip8::getPixelMap()
{
	return pixelMap;
}
