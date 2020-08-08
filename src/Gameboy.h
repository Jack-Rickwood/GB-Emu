#include "Registers.h"
#include "Memory.h"
#include "GPU.h"
#include "CPU.h"
#pragma once

namespace GBEmu {
	class Gameboy {
	public:
		// Debug
		bool debug = false;

		// Hardware
		GBEmu::CPU::Registers registers;
		GBEmu::CPU::Opcodes opcodes;
		GBEmu::MMU bus;
		GBEmu::GPU gpu;
		GBEmu::CPU::CPU cpu;

		// Functions
		void LoadRom(std::string rompath, bool debug_mode);
		void Update(GBEmu::Gameboy* gameboy);
	};
}