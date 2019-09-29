#include "Chip8.h"
#include <fstream>
#include <iostream>


Chip8::Chip8()
	: opcode(0), memory(), vRegister(), indexRegister(0)
	, programCounter(0x0u), pixelMap(), delayTimer(0), soundTimer(0)
	, stack(), keys(), drawFlag(false), opcodeMap()
{
	opcodeMap[0x0u] = [this]()
	{
		switch (opcode & 0x000F)
		{
		case 0x0000:
			for (bool& pixel : pixelMap)
			{
				pixel = false;
			}
			drawFlag = true;
			programCounter += 2;
			break;

		case 0x000E:
			programCounter = stack.top();
			stack.pop();
			programCounter += 2;
			break;

		default:
			break;
		}
	};

	opcodeMap[0x1u] = [this]()
	{
		programCounter = opcode & 0x0FFF;
	};

	opcodeMap[0x2u] = [this]()
	{
		stack.push(programCounter);
		programCounter = opcode & 0x0FFF;
	};

	opcodeMap[0x3u] = [this]()
	{
		if (vRegister[(opcode & 0x0F00) >> 8] == (opcode & 0x0FF))
			programCounter += 4;
		else
			programCounter += 2;
	};

	opcodeMap[0xAu] = [this]()
	{
		indexRegister = opcode & 0x0FFF;
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

	uint8_t mapKey = (opcode & 0xF000) >> 12;

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
