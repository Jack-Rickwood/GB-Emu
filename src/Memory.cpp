#include <iostream>
#include <stdio.h>
#include <fstream>
#include "Gameboy.h"
#include "Memory.h"
#include "GPU.h"
#include "Constants.h"

using namespace std;
using namespace GBEmu;

uint8_t MMU::ReadByte(GBEmu::Gameboy* gameboy, uint16_t address) {
	return memory.memory_array[address];
}

uint16_t MMU::ReadWord(uint16_t address) {
	uint16_t word = ((uint16_t)memory.memory_array[address + 1] << 8) | (uint16_t)memory.memory_array[address];
	return word;
}

void MMU::WriteByte(GBEmu::Gameboy* gameboy, uint16_t address, uint8_t value) {
	if (address == Constants::DIVIDER_REGISTER_ADDRESS) {
		memory.memory_array[address] = 0;
	}
	else if (address == Constants::LCD_Y_COORD_ADDRESS) {
		memory.memory_array[address] = 0;
	}
	else if (address == Constants::TIMER_CONTROLLER_ADDRESS) {
		uint8_t current_frequency = gameboy->cpu.GetClockFreq(gameboy);
		memory.memory_array[address] = value;
		uint8_t new_frequency = gameboy->cpu.GetClockFreq(gameboy);
		if (current_frequency != new_frequency) {
			gameboy->cpu.SetClockFreq(gameboy);
		}
	}
	else if (address == 0xFF46) {
		DoDMATransfer(gameboy, address);
	}
	else {
		memory.memory_array[address] = value;
	}
}

void MMU::WriteWord(uint16_t address, uint16_t value) {
	memory.memory_array[address + 1] = (uint8_t)(value >> 8);
	memory.memory_array[address] = (uint8_t)value;
}

void MMU::DoDMATransfer(GBEmu::Gameboy* gameboy, uint8_t value) {
	uint8_t address = value << 8;
	for (int i = 0; i < 0xA0; i++) {
		WriteByte(gameboy, 0xFE00 + i, ReadByte(gameboy, address + i));
	}
}

uint8_t MMU::BitSet(uint8_t value, size_t bit_position) {
	uint8_t mask = 0b00000001 << bit_position;
	value |= mask;
	return value;
}

uint8_t MMU::BitReset(uint8_t value, size_t bit_position) {
	uint8_t mask = 0b00000001 << bit_position;
	value &= ~mask;
	return value;
}

uint8_t MMU::BitToggle(uint8_t value, size_t bit_position) {
	uint8_t mask = 0b00000001 << bit_position;
	value ^= mask;
	return value;
}

bool MMU::BitTest(uint8_t value, size_t bit_position) {
	uint8_t mask = 0b00000001;
	uint8_t shifted_value = value >> bit_position;
	return (mask & shifted_value);
}

void MMU::Initialize(GBEmu::Gameboy* gameboy) {
	gameboy->cpu.pc = GBEmu::Constants::ROM_START_ADDRESS;
	gameboy->registers.SetAF(0x01B0);
	gameboy->registers.SetBC(0x0013);
	gameboy->registers.SetDE(0x00D8);
	gameboy->registers.SetHL(0x014D);
	gameboy->registers.flags_registers.zero = true;
	gameboy->registers.flags_registers.subtract = false;
	gameboy->registers.flags_registers.halfcarry = true;
	gameboy->registers.flags_registers.carry = true;
	gameboy->registers.SetByteFromFlags();
	gameboy->cpu.sp = 0xFFFE;
	memory.memory_array[0xFF05] = 0x00;
	memory.memory_array[0xFF06] = 0x00;
	memory.memory_array[0xFF07] = 0x00;
	memory.memory_array[0xFF10] = 0x80;
	memory.memory_array[0xFF11] = 0xBF;
	memory.memory_array[0xFF12] = 0xF3;
	memory.memory_array[0xFF14] = 0xBF;
	memory.memory_array[0xFF16] = 0x3F;
	memory.memory_array[0xFF17] = 0x00;
	memory.memory_array[0xFF19] = 0xBF;
	memory.memory_array[0xFF1A] = 0x7F;
	memory.memory_array[0xFF1B] = 0xFF;
	memory.memory_array[0xFF1C] = 0x9F;
	memory.memory_array[0xFF1E] = 0xBF;
	memory.memory_array[0xFF20] = 0xFF;
	memory.memory_array[0xFF21] = 0x00;
	memory.memory_array[0xFF22] = 0x00;
	memory.memory_array[0xFF23] = 0xBF;
	memory.memory_array[0xFF24] = 0x77;
	memory.memory_array[0xFF25] = 0xF3;
	memory.memory_array[0xFF26] = 0xF1;
	memory.memory_array[0xFF40] = 0x91;
	memory.memory_array[0xFF42] = 0x00;
	memory.memory_array[0xFF43] = 0x00;
	memory.memory_array[0xFF45] = 0x00;
	memory.memory_array[0xFF47] = 0xFC;
	memory.memory_array[0xFF48] = 0xFF;
	memory.memory_array[0xFF49] = 0xFF;
	memory.memory_array[0xFF4A] = 0x00;
	memory.memory_array[0xFF4B] = 0x00;
	memory.memory_array[0xFFFF] = 0x00;
}

void MMU::LoadRom(GBEmu::Gameboy* gameboy, string path) {
	ifstream rom;
	size_t size = 0;
	rom.open(path, ios::in | ios::binary | ios::ate);

	// Get rom length.
	rom.seekg(0, ios::end);
	size = rom.tellg();
	rom.seekg(0, ios::beg);

	rom_data = new uint8_t[size];
	rom.read((char*)rom_data, size);
	rom.close();

	// Get ROM Title
	for (int i = 0; i <= 15; i++) {
		char letter = rom_data[0x0134 + i];
		if(letter != 0) {
			rom_header.title += letter;
		}
	}
	cout << "[INFO] " << "ROM Name: " << rom_header.title << endl;

	// Get ROM MBC Type
	switch (rom_data[0x0147]) {
	case 0x00:
		rom_header.mbc_type = MBC::ROM_ONLY;
		break;
	case 0x01:
		rom_header.mbc_type = MBC::MBC1;
		break;
	case 0x02:
		rom_header.mbc_type = MBC::MBC1_RAM;
		break;
	case 0x03:
		rom_header.mbc_type = MBC::MBC1_RAM_BATTERY;
		break;
	case 0x05:
		rom_header.mbc_type = MBC::MBC2;
		break;
	case 0x06:
		rom_header.mbc_type = MBC::MBC2_BATTERY;
		break;
	case 0x08:
		rom_header.mbc_type = MBC::ROM_RAM;
		break;
	case 0x09:
		rom_header.mbc_type = MBC::ROM_RAM_BATTERY;
		break;
	case 0x0B:
		rom_header.mbc_type = MBC::MMM1;
		break;
	case 0x0C:
		rom_header.mbc_type = MBC::MMM1_RAM;
		break;
	case 0x0D:
		rom_header.mbc_type = MBC::MMM1_RAM_BATTERY;
		break;
	case 0x0F:
		rom_header.mbc_type = MBC::MBC3_TIMER_BATTERY;
		break;
	case 0x10:
		rom_header.mbc_type = MBC::MBC3_TIMER_RAM_BATTERY;
		break;
	case 0x11:
		rom_header.mbc_type = MBC::MBC3;
		break;
	case 0x12:
		rom_header.mbc_type = MBC::MBC3_RAM;
		break;
	case 0x13:
		rom_header.mbc_type = MBC::MBC3_RAM_BATTERY;
		break;
	case 0x19:
		rom_header.mbc_type = MBC::MBC5;
		break;
	case 0x1A:
		rom_header.mbc_type = MBC::MBC5_RAM;
		break;
	case 0x1B:
		rom_header.mbc_type = MBC::MBC5_RAM_BATTERY;
		break;
	case 0x1C:
		rom_header.mbc_type = MBC::MBC5_RUMBLE;
		break;
	case 0x1D:
		rom_header.mbc_type = MBC::MBC5_RUMBLE_RAM;
		break;
	case 0x1E:
		rom_header.mbc_type = MBC::MBC5_RUMBLE_RAM_BATTERY;
		break;
	case 0x20:
		rom_header.mbc_type = MBC::MBC6;
		break;
	case 0x22:
		rom_header.mbc_type = MBC::MBC7_SENSOR_RUMBLE_RAM_BATTERY;
		break;
	case 0xFC:
		rom_header.mbc_type = MBC::POCKET_CAMERA;
		break;
	case 0xFD:
		rom_header.mbc_type = MBC::BANDAI_TAMA5;
		break;
	case 0xFE:
		rom_header.mbc_type = MBC::HUC3;
		break;
	case 0xFF:
		rom_header.mbc_type = MBC::HUC1_RAM_BATTERY;
		break;
	}

	// Get ROM Size
	switch (rom_data[0x0148]) {
	case 0x00:
		rom_header.rom_size = 1024 * 32;
		rom_header.number_of_rom_banks = 1;
		break;
	case 0x01:
		rom_header.rom_size = 1024 * 64;
		rom_header.number_of_rom_banks = 4;
		break;
	case 0x02:
		rom_header.rom_size = 1024 * 128;
		rom_header.number_of_rom_banks = 8;
		break;
	case 0x03:
		rom_header.rom_size = 1024 * 256;
		rom_header.number_of_rom_banks = 16;
		break;
	case 0x04:
		rom_header.rom_size = 1024 * 512;
		rom_header.number_of_rom_banks = 32;
		break;
	case 0x05:
		rom_header.rom_size = 1024 * 1024;
		rom_header.number_of_rom_banks = 64;
		break;
	case 0x06:
		rom_header.rom_size = 1024 * 1024 * 2;
		rom_header.number_of_rom_banks = 128;
		break;
	case 0x07:
		rom_header.rom_size = 1024 * 1024 * 4;
		rom_header.number_of_rom_banks = 256;
		break;
	case 0x08:
		rom_header.rom_size = 1024 * 1024 * 8;
		rom_header.number_of_rom_banks = 512;
		break;
	case 0x52:
		rom_header.rom_size = 1024.0 * 1024.0 * 1.1;
		rom_header.number_of_rom_banks = 72;
		break;
	case 0x53:
		rom_header.rom_size = 1024.0 * 1024.0 * 1.2;
		rom_header.number_of_rom_banks = 80;
		break;
	case 0x54:
		rom_header.rom_size = 1024.0 * 1024.0 * 1.5;
		rom_header.number_of_rom_banks = 96;
		break;
	}
	cout << "[INFO] " << "ROM Size: " << rom_header.rom_size << " bytes, with " << rom_header.number_of_rom_banks << " banks." << endl;

	// Get External RAM Size
	switch (rom_data[0x0149]) {
	case 0x00:
		rom_header.ram_size = 0;
		rom_header.number_of_ram_banks = 0;
		break;
	case 0x01:
		rom_header.ram_size = 1024 * 2;
		rom_header.number_of_ram_banks = 1;
		break;
	case 0x02:
		rom_header.ram_size = 1024 * 8;
		rom_header.number_of_ram_banks = 1;
		break;
	case 0x03:
		rom_header.ram_size = 1024 * 32;
		rom_header.number_of_ram_banks = 4;
		break;
	case 0x04:
		rom_header.ram_size = 1024 * 128;
		rom_header.number_of_ram_banks = 16;
		break;
	case 0x05:
		rom_header.ram_size = 1024 * 64;
		rom_header.number_of_ram_banks = 8;
		break;
	}
	cout << "[INFO] " << "External RAM Size: " << rom_header.ram_size << " bytes, with " << rom_header.number_of_ram_banks << " banks." << endl;

	// Get ROM Country
	switch (rom_data[0x014A]) {
	case 0x00:
		rom_header.country = "Japanese";
		break;
	case 0x01:
		rom_header.country = "Non-Japanese";
		break;
	}
	cout << "[INFO] " << "ROM Country: " << rom_header.country << endl;

	// Get ROM Version
	rom_header.version = rom_data[0x014C];
	cout << "[INFO] " << "ROM Version: " << rom_header.version << endl;

	// Write Bank 0
	if (GetGeneralMBC(rom_header.mbc_type) == GeneralMBC::MBC1 ||
		GetGeneralMBC(rom_header.mbc_type) == GeneralMBC::MBC2 ||
		GetGeneralMBC(rom_header.mbc_type) == GeneralMBC::MBC3 ||
		GetGeneralMBC(rom_header.mbc_type) == GeneralMBC::MBC5) {
		for (int i = 0x0000; i < 0x4000; i++) {
			WriteByte(gameboy, i, rom_data[i]);
		}
	}
	else if (GetGeneralMBC(rom_header.mbc_type) == GeneralMBC::ROM_ONLY) {
		for (int i = 0x0000; i < 0x8000; i++) {
			WriteByte(gameboy, i, rom_data[i]);
		}
	}	
	else {
		cerr << "[ERROR] " << "Unsupported MBC." << endl;
		exit(1);
	}
}

MMU::GeneralMBC MMU::GetGeneralMBC(MBC mbc) {
	if (mbc == MBC::ROM_ONLY) {
		return GeneralMBC::ROM_ONLY;
	}
	
	else if (mbc == MBC::MBC1 || mbc == MBC::MBC1_RAM || mbc == MBC::MBC1_RAM_BATTERY) {
		return GeneralMBC::MBC1;
	}
	
	else if (mbc == MBC::MBC2 || mbc == MBC::MBC2_BATTERY) {
		return GeneralMBC::MBC2;
	}
	
	else if (mbc == MBC::MBC3_TIMER_BATTERY || mbc == MBC::MBC3_TIMER_RAM_BATTERY ||
		mbc == MBC::MBC3 || mbc == MBC::MBC3_RAM || mbc == MBC::MBC3_RAM_BATTERY) {
		return GeneralMBC::MBC3;
	}
	
	else if (mbc == MBC::MBC5 || mbc == MBC::MBC5_RAM || mbc == MBC::MBC5_RAM_BATTERY ||
		mbc == MBC::MBC5_RUMBLE || mbc == MBC::MBC5_RUMBLE_RAM || mbc == MBC::MBC5_RUMBLE_RAM_BATTERY) {
		return GeneralMBC::MBC5;
	}
	
	else {
		cerr << "[ERROR] " << "Unsupported MBC." << endl;
		exit(1);
	}
}