#include "Constants.h"
#pragma once

namespace GBEmu {
	class Gameboy;
	class MMU {
	public:
		// Emulates the gameboy's Memory
		struct memory {
			uint8_t memory_array[GBEmu::Constants::RAM_SIZE];
		} memory;

		// Enum containing all possible MBC types
		enum class MBC {
			ROM_ONLY,
			MBC1,
			MBC1_RAM,
			MBC1_RAM_BATTERY,
			MBC2,
			MBC2_BATTERY,
			ROM_RAM,
			ROM_RAM_BATTERY,
			MMM1,
			MMM1_RAM,
			MMM1_RAM_BATTERY,
			MBC3_TIMER_BATTERY,
			MBC3_TIMER_RAM_BATTERY,
			MBC3,
			MBC3_RAM,
			MBC3_RAM_BATTERY,
			MBC5,
			MBC5_RAM,
			MBC5_RAM_BATTERY,
			MBC5_RUMBLE,
			MBC5_RUMBLE_RAM,
			MBC5_RUMBLE_RAM_BATTERY,
			MBC6,
			MBC7_SENSOR_RUMBLE_RAM_BATTERY,
			POCKET_CAMERA,
			BANDAI_TAMA5,
			HUC3,
			HUC1_RAM_BATTERY
		};
		// Enum containing the general MBC's I hope to implement, as 
		// implementing every individual MBC is currently not a priority
		enum class GeneralMBC {
			ROM_ONLY,
			MBC1,
			MBC2,
			MBC3,
			MBC5,
		};

		// Vars
		struct rom_header {
			std::string title; // 0134-0143 - Title
			MBC mbc_type; // 0147 - Cartridge Type
			size_t rom_size; // 0148 - ROM Size
			int number_of_rom_banks;
			size_t ram_size; // 0149 - RAM Size
			int number_of_ram_banks;
			std::string country; // 014A - Destination Code
			int version; // 014C - Mask ROM Version number
		} rom_header;
		uint8_t* rom_data;

		// Functions
		uint8_t ReadByte(GBEmu::Gameboy* gameboy, uint16_t address);
		uint16_t ReadWord(uint16_t address);
		void WriteByte(GBEmu::Gameboy* gameboy, uint16_t address, uint8_t value);
		void WriteWord(uint16_t address, uint16_t value);
		uint8_t BitSet(uint8_t value, size_t bit_position);
		uint8_t BitReset(uint8_t value, size_t bit_position);
		uint8_t BitToggle(uint8_t value, size_t bit_position);
		bool BitTest(uint8_t value, size_t bit_position);
		void DoDMATransfer(GBEmu::Gameboy* gameboy, uint8_t value);
		void Initialize(GBEmu::Gameboy* gameboy);
		void LoadRom(GBEmu::Gameboy* gameboy, std::string path);
		GeneralMBC GetGeneralMBC(MBC mbc);
	};
}