#include "Chip8.h"
#include <fstream>
#include <random>
#include <iostream>


Chip8::Chip8()
	: opcode(0), memory(), vRegister(), indexRegister(0)
	, programCounter(0x0u), pixelMap(), delayTimer(0), soundTimer(0)
	, stack(), keys(), drawFlag(false), opcodeMap(), opcodeMap0()
	, opcodeMap8(), opcodeMapE(), opcodeMapF(), waitingForKey(false)
	, interrupt(false)
{
	for (bool& pixel : pixelMap)
	{
		pixel = false;
	}

	opcodeMap[0x0000] = [this]() { opcodeMap0[opcode & 0x000F](); };
	opcodeMap[0x1000] = [this]() { programCounter = opcode & 0x0FFF; };
	opcodeMap[0x2000] = [this]() { stack.push(programCounter); programCounter = opcode & 0x0FFF; };
	opcodeMap[0x3000] = [this]() { programCounter += vRegister[(opcode & 0x0F00) >> 8] == (opcode & 0x0FF) ? 4 : 2; };
	opcodeMap[0x4000] = [this]() { programCounter += vRegister[(opcode & 0x0F00) >> 8] != (opcode & 0x0FF) ? 4 : 2; };
	opcodeMap[0x5000] = [this]() { programCounter += vRegister[(opcode & 0x0F00) >> 8] == vRegister[(opcode & 0x00F0 >> 4)] ? 4 : 2; };
	opcodeMap[0x6000] = [this]() { vRegister[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF); programCounter += 2; };
	opcodeMap[0x7000] = [this]() { vRegister[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF); programCounter += 2; };
	opcodeMap[0x8000] = [this]() { opcodeMap8[opcode & 0x000F](); programCounter += 2; };
	opcodeMap[0x9000] = [this]() { programCounter += vRegister[(opcode & 0x0F00) >> 8] != vRegister[(opcode & 0x00F0 >> 4)] ? 4 : 2; };
	opcodeMap[0xA000] = [this]() { indexRegister = opcode & 0x0FFF; programCounter += 2; };
	opcodeMap[0xB000] = [this]() { programCounter = vRegister[0x0] + (opcode & 0x0FFF); };
	opcodeMap[0xC000] = [this]()
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(0.0, std::nextafter(255.0, DBL_MAX));

		vRegister[(opcode & 0x0F00) >> 8] = (int)dist(mt) & vRegister[(opcode & 0x0F00) >> 8];

		programCounter += 2;
	};
	opcodeMap[0xD000] = [this]()
	{
		uint8_t x = vRegister[(opcode & 0x0F00) >> 8];
		uint8_t y = vRegister[(opcode & 0x00F0) >> 4];
		uint8_t height = opcode & 0x000F;
		uint8_t pixelLine;

		for (uint8_t xline = 0; xline < 8; xline++)
		{
			pixelLine = memory[indexRegister + xline];
			for (uint8_t yline = 0; yline < height; yline++)
			{
				if ((pixelLine & 0x80) >> xline != 0)
				{
					if (pixelMap[(x + xline) + ((y + yline) * 64)])
						vRegister[0xF] = 1;
					pixelMap[(x + xline) + ((y + yline) * 64)] ^= false;
				}
			}
		}

		drawFlag = true;
		programCounter += 2;
	};
	opcodeMap[0xE000] = [this]() { opcodeMapE[opcode & 0x00FF](); programCounter += 2; };
	opcodeMap[0xF000] = [this]() { opcodeMapE[opcode & 0x00FF](); programCounter += 2; };

	//// opcodes starting with 0x0, differentiated by the last 4 bits
	opcodeMap0[0x0000] = [this]()
	{
		for (bool& pixel : pixelMap)
		{
			pixel = false;
		}
		drawFlag = true;
		programCounter += 2;
	};
	opcodeMap0[0x000E] = [this]()
	{
		programCounter = stack.top();
		stack.pop();
		programCounter += 2;
	};

	//// opcodes starting with 0x8, differentiated by the last 4 bits
	opcodeMap8[0x0000] = [this]() { vRegister[(opcode & 0x0F00) >> 8]  = vRegister[(opcode & 0x00F0) >> 4]; };
	opcodeMap8[0x0001] = [this]() { vRegister[(opcode & 0x0F00) >> 8] |= vRegister[(opcode & 0x00F0) >> 4]; };
	opcodeMap8[0x0002] = [this]() { vRegister[(opcode & 0x0F00) >> 8] &= vRegister[(opcode & 0x00F0) >> 4]; };
	opcodeMap8[0x0003] = [this]() { vRegister[(opcode & 0x0F00) >> 8] ^= vRegister[(opcode & 0x00F0) >> 4]; };
	opcodeMap8[0x0004] = [this]()
	{
		uint8_t x = (opcode & 0x0F00) >> 8;
		uint8_t y = (opcode & 0x00F0) >> 4;

		// carry flag
		vRegister[0xF] = (vRegister[y] > 0xFF - vRegister[x]) ? 1 : 0;

		vRegister[x] += vRegister[y];
	};
	opcodeMap8[0x0005] = [this]()
	{
		uint8_t x = (opcode & 0x0F00) >> 8;
		uint8_t y = (opcode & 0x00F0) >> 4;

		// carry flag
		vRegister[0xF] = (vRegister[x] > vRegister[y]) ? 1 : 0;

		vRegister[x] -= vRegister[y];
	};
	opcodeMap8[0x0006] = [this]()
	{
		uint8_t x = (opcode & 0x0F00) >> 8;

		// carry flag
		vRegister[0xF] = vRegister[x] & 0x1;

		vRegister[x] = vRegister[x] >> 1;
	};
	opcodeMap8[0x0007] = [this]()
	{
		uint8_t x = (opcode & 0x0F00) >> 8;
		uint8_t y = (opcode & 0x00F0) >> 4;

		// carry flag
		vRegister[0xF] = (vRegister[y] > vRegister[x]) ? 1 : 0;

		vRegister[x] = vRegister[y] - vRegister[x];
	};
	opcodeMap8[0x0008] = [this]()
	{
		uint8_t x = (opcode & 0x0F00) >> 8;

		// carry flag
		// TODO: maybe 0x10 instead of 0x80?
		vRegister[0xF] = vRegister[x] & 0x80;

		vRegister[x] = vRegister[x] << 1;
	};

	//// opcodes starting with 0xE, differentiated by the last 8 bits
	opcodeMapE[0x009E] = [this]() { if (keys[vRegister[(opcode & 0x0F00) >> 8]]) programCounter += 2; };
	opcodeMapE[0x00A1] = [this]() { if (!keys[vRegister[(opcode & 0x0F00) >> 8]]) programCounter += 2; };

	//// opcodes starting with 0xF, differentiated by the last 8 bits
	opcodeMapF[0x000A] = [this]()
	{

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

	if (waitingForKey)
		return;

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

void Chip8::setKeys(const std::array<bool, 16>& keys)
{
	this->keys = keys;
	waitingForKey = false;
}