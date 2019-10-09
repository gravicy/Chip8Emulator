#include "Chip8.h"

#include <fstream>
#include <random>
#include <iostream>
#include <algorithm>
#include <cfloat>

Chip8::Chip8()
	: opcode(0), memory(), vRegister(), indexRegister(0x200)
	, programCounter(0x200), pixelMap(), delayTimer(0), soundTimer(0)
	, stack(), keys(), drawFlag(false), opcodeMap(), opcodeMap0()
	, opcodeMap8(), opcodeMapE(), opcodeMapF()
{
	for (auto& pixelArray : pixelMap)
		for (bool& pixel : pixelArray)
			pixel = false;
	
	for (bool& key : keys)
		key = false;

	initOpCodeMaps();

	std::copy(std::begin(chip8_fontset), std::end(chip8_fontset), std::begin(memory));
}

void Chip8::loadGame(std::string filename)
{
	std::ifstream file;
	file.open(filename, std::ios::binary | std::ios::ate);

	if (!file.fail() && file.good())
	{
		auto size = file.tellg();
		file.seekg(0, file.beg);
		file.read((char*)&memory[indexRegister], size);

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

	if (delayTimer > 0)
		delayTimer--;
	
	if (soundTimer > 0)
		soundTimer--;
}

bool Chip8::getDrawFlag()
{
	return drawFlag;
}

const std::array<std::array<bool, 32>, 64>& Chip8::getPixelMap()
{
	return pixelMap;
}

std::array<bool, 16>& Chip8::getKeys()
{
	return keys;
}

void Chip8::initOpCodeMaps()
{
	opcodeMap[0x0000] = [this]() { opcodeMap0[opcode & 0x000F](); programCounter += 2; };
	opcodeMap[0x1000] = [this]() { programCounter = opcode & 0x0FFF; };
	opcodeMap[0x2000] = [this]() { stack.push(programCounter); programCounter = opcode & 0x0FFF; };
	opcodeMap[0x3000] = [this]() { programCounter += vRegister[(opcode & 0x0F00) >> 8] == (opcode & 0x0FF) ? 4 : 2; };
	opcodeMap[0x4000] = [this]() { programCounter += vRegister[(opcode & 0x0F00) >> 8] != (opcode & 0x0FF) ? 4 : 2; };
	opcodeMap[0x5000] = [this]() { programCounter += vRegister[(opcode & 0x0F00) >> 8] == vRegister[(opcode & 0x00F0 >> 4)] ? 4 : 2; };
	opcodeMap[0x6000] = [this]() { vRegister[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF); programCounter += 2; };
	opcodeMap[0x7000] = [this]() { vRegister[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF); programCounter += 2; };
	opcodeMap[0x8000] = [this]() { opcodeMap8[opcode & 0x000F]((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4); programCounter += 2; };
	opcodeMap[0x9000] = [this]() { programCounter += vRegister[(opcode & 0x0F00) >> 8] != vRegister[(opcode & 0x00F0 >> 4)] ? 4 : 2; };
	opcodeMap[0xA000] = [this]() { indexRegister = opcode & 0x0FFF; programCounter += 2; };
	opcodeMap[0xB000] = [this]() { programCounter = vRegister[0x0] + (opcode & 0x0FFF); };
	opcodeMap[0xC000] = [this]()
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(0.0, std::nextafter(255.0, DBL_MAX));

		auto num = (int)dist(mt);
		vRegister[(opcode & 0x0F00) >> 8] = num & vRegister[(opcode & 0x0F00) >> 8];

		programCounter += 2;
	};
	opcodeMap[0xD000] = [this]()
	{
		uint8_t x = vRegister[(opcode & 0x0F00) >> 8];
		uint8_t y = vRegister[(opcode & 0x00F0) >> 4];
		uint8_t height = opcode & 0x000F;
		uint8_t pixelLine;

		for (uint8_t yline = 0; yline < height; yline++)
		{
			pixelLine = memory[indexRegister + yline];
			for (uint8_t xline = 0; xline < 8; xline++)
			{
				if ((pixelLine & (0x80 >> xline)) != 0)
				{
					if (pixelMap[x + xline][y + yline])
						vRegister[0xF] = 1;
					pixelMap[x + xline][y + yline] ^= true;
				}
			}
		}

		drawFlag = true;
		programCounter += 2;
	};
	opcodeMap[0xE000] = [this]() { opcodeMapE[opcode & 0x00FF]((opcode & 0x0F00) >> 8); programCounter += 2; };
	opcodeMap[0xF000] = [this]() { opcodeMapF[opcode & 0x00FF]((opcode & 0x0F00) >> 8); programCounter += 2; };

	//// opcodes starting with 0x0, differentiated by the last 4 bits
	opcodeMap0[0x0000] = [this]() { for (auto& pixelArray : pixelMap) for (bool& pixel : pixelArray) pixel = false; drawFlag = true; };
	opcodeMap0[0x000E] = [this]() { programCounter = stack.top(); stack.pop(); };

	//// opcodes starting with 0x8, differentiated by the last 4 bits
	opcodeMap8[0x0000] = [this](int x, int y) { vRegister[x] = vRegister[y]; };
	opcodeMap8[0x0001] = [this](int x, int y) { vRegister[x] |= vRegister[y]; };
	opcodeMap8[0x0002] = [this](int x, int y) { vRegister[x] &= vRegister[y]; };
	opcodeMap8[0x0003] = [this](int x, int y) { vRegister[x] ^= vRegister[y]; };
	opcodeMap8[0x0004] = [this](int x, int y) { vRegister[0xF] = (vRegister[y] > 0xFF - vRegister[x]) ? 1 : 0; vRegister[x] += vRegister[y]; };
	opcodeMap8[0x0005] = [this](int x, int y) { vRegister[0xF] = (vRegister[x] > vRegister[y]) ? 1 : 0;	vRegister[x] -= vRegister[y]; };
	opcodeMap8[0x0006] = [this](int x, int y) { vRegister[0xF] = vRegister[x] & 0x1; vRegister[x] = vRegister[x] >> 1; };
	opcodeMap8[0x0007] = [this](int x, int y) { vRegister[0xF] = (vRegister[y] > vRegister[x]) ? 1 : 0; vRegister[x] = vRegister[y] - vRegister[x]; };
	opcodeMap8[0x000E] = [this](int x, int y) { /*carry flag*/ vRegister[0xF] = vRegister[x] & 0x80; vRegister[x] = vRegister[x] << 1; };

	//// opcodes starting with 0xE, differentiated by the last 8 bits
	opcodeMapE[0x009E] = [this](int x) { if (keys[vRegister[x]]) programCounter += 2; };
	opcodeMapE[0x00A1] = [this](int x) { if (!keys[vRegister[x]]) programCounter += 2; };

	//// opcodes starting with 0xF, differentiated by the last 8 bits
	opcodeMapF[0x0007] = [this](int x) { vRegister[(opcode & 0x0F00) >> 8] = delayTimer; };
	opcodeMapF[0x000A] = [this](int x)
	{
		for (auto i = 0; i < keys.size(); i++)
		{
			if (keys[i])
			{
				vRegister[x] = i;
				return;
			}
		}
		programCounter -= 2;
	};
	opcodeMapF[0x0015] = [this](int x) { delayTimer = vRegister[(opcode & 0x0F00) >> 8]; };
	opcodeMapF[0x0018] = [this](int x) { soundTimer = vRegister[(opcode & 0x0F00) >> 8]; };
	opcodeMapF[0x001E] = [this](int x)
	{
		if (indexRegister + vRegister[x] > 0xFFF)
			vRegister[0xF] = 1;
		indexRegister = (indexRegister + vRegister[x]) & 0x0FFF;
	};
	opcodeMapF[0x0029] = [this](int x) { indexRegister = vRegister[x] * 5; };
	opcodeMapF[0x0033] = [this](int x)
	{
		uint8_t vx = vRegister[x];
		memory[indexRegister + 0] = vx / 100;
		memory[indexRegister + 1] = (vx / 10) % 10;
		memory[indexRegister + 2] = vx % 10;
	};
	opcodeMapF[0x0055] = [this](int x) { for (uint8_t i = 0; i <= x; i++) memory[indexRegister + i] = vRegister[i]; };
	opcodeMapF[0x0065] = [this](int x) { for (uint8_t i = 0; i <= x; i++) vRegister[i] = memory[indexRegister + i]; };
}