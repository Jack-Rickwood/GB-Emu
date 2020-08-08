#include <iostream>
#include <stdio.h>
#include "CPU.h"
#include "Gameboy.h"

using namespace std;
using namespace GBEmu;

/* INC */
void CPU::Opcodes::opcode_inc_set_flags(GBEmu::Gameboy* gameboy, uint8_t value) {
	// Flags Effected
	gameboy->registers.flags_registers.zero = value == 0;
	gameboy->registers.flags_registers.subtract = false;
	gameboy->registers.flags_registers.halfcarry = (((value - 1) & 0x0F) + (value & 0x0F)) > 0x0F;
	gameboy->registers.SetByteFromFlags();
}

void CPU::Opcodes::opcode_inc_hl_deref(GBEmu::Gameboy* gameboy) {
	uint16_t address = gameboy->registers.GetHL();
	uint8_t value = gameboy->bus.ReadByte(gameboy, address) + 1;
	gameboy->bus.WriteByte(gameboy, address, value);

	// Flags Effected
	gameboy->registers.flags_registers.zero = value == 0;
	gameboy->registers.flags_registers.subtract = false;
	gameboy->registers.flags_registers.halfcarry = (((value - 1) & 0x0F) + (value & 0x0F)) > 0x0F;
	gameboy->registers.SetByteFromFlags();
}

/* DEC */
void CPU::Opcodes::opcode_dec_set_flags(GBEmu::Gameboy* gameboy, uint8_t value) {
	// Flags Effected
	gameboy->registers.flags_registers.zero = value == 0;
	gameboy->registers.flags_registers.subtract = true;
	gameboy->registers.flags_registers.halfcarry = (((value + 1) & 0x0F) - (value & 0x0F)) < 0;
	gameboy->registers.SetByteFromFlags();
}

void CPU::Opcodes::opcode_dec_hl_deref(GBEmu::Gameboy* gameboy) {
	uint16_t address = gameboy->registers.GetHL();
	uint8_t value = gameboy->bus.ReadByte(gameboy, address) - 1;
	gameboy->bus.WriteByte(gameboy, address, value);

	// Flags Effected
	gameboy->registers.flags_registers.zero = value == 0;
	gameboy->registers.flags_registers.subtract = true;
	gameboy->registers.flags_registers.halfcarry = (((value + 1) & 0x0F) - (value & 0x0F)) < 0;
	gameboy->registers.SetByteFromFlags();
}

/* CPL */
void CPU::Opcodes::opcode_cpl(GBEmu::Gameboy* gameboy) {
	gameboy->registers.registers.a = ~gameboy->registers.registers.a;
	
	// Flags Effected
	gameboy->registers.flags_registers.subtract = true;
	gameboy->registers.flags_registers.halfcarry = true;
	gameboy->registers.SetByteFromFlags();
}

/* LD */
void CPU::Opcodes::opcode_ld_a(GBEmu::Gameboy* gameboy, uint8_t value) {
	gameboy->registers.registers.a = value;
	// No Flags Effected
}

void CPU::Opcodes::opcode_ld_b(GBEmu::Gameboy* gameboy, uint8_t value) {
	gameboy->registers.registers.b = value;
	// No Flags Effected
}

void CPU::Opcodes::opcode_ld_c(GBEmu::Gameboy* gameboy, uint8_t value) {
	gameboy->registers.registers.c = value;
	// No Flags Effected
}

void CPU::Opcodes::opcode_ld_d(GBEmu::Gameboy* gameboy, uint8_t value) {
	gameboy->registers.registers.d = value;
	// No Flags Effected
}

void CPU::Opcodes::opcode_ld_e(GBEmu::Gameboy* gameboy, uint8_t value) {
	gameboy->registers.registers.e = value;
	// No Flags Effected
}

void CPU::Opcodes::opcode_ld_h(GBEmu::Gameboy* gameboy, uint8_t value) {
	gameboy->registers.registers.h = value;
	// No Flags Effected
}

void CPU::Opcodes::opcode_ld_l(GBEmu::Gameboy* gameboy, uint8_t value) {
	gameboy->registers.registers.l = value;
	// No Flags Effected
}

void CPU::Opcodes::opcode_ld_bc(GBEmu::Gameboy* gameboy) {
	gameboy->registers.SetBC(gameboy->bus.ReadWord(gameboy->cpu.pc + 1));
	// No Flags Effected
}

void CPU::Opcodes::opcode_ld_bc_deref(GBEmu::Gameboy* gameboy, uint8_t value) {
	gameboy->bus.WriteByte(gameboy, gameboy->registers.GetBC(), value);
	// No Flags Effected
}

void CPU::Opcodes::opcode_ld_de(GBEmu::Gameboy* gameboy) {
	gameboy->registers.SetDE(gameboy->bus.ReadWord(gameboy->cpu.pc + 1));
	// No Flags Effected
}

void CPU::Opcodes::opcode_ld_de_deref(GBEmu::Gameboy* gameboy, uint8_t value) {
	gameboy->bus.WriteByte(gameboy, gameboy->registers.GetDE(), value);
	// No Flags Effected
}

void CPU::Opcodes::opcode_ld_hl(GBEmu::Gameboy* gameboy) {
	gameboy->registers.SetHL(gameboy->bus.ReadWord(gameboy->cpu.pc + 1));
	// No Flags Effected
}

void CPU::Opcodes::opcode_ld_hl_deref(GBEmu::Gameboy* gameboy, uint8_t value) {
	gameboy->bus.WriteByte(gameboy, gameboy->registers.GetHL(), value);
	// No Flags Effected
}

void CPU::Opcodes::opcode_ld_sp(GBEmu::Gameboy* gameboy) {
	gameboy->cpu.sp = gameboy->bus.ReadWord(gameboy->cpu.pc + 1);
	// No Flags Effected
}

void CPU::Opcodes::opcode_ld_mem(GBEmu::Gameboy* gameboy, uint16_t value) {
	uint16_t address = gameboy->bus.ReadWord(gameboy->cpu.pc + 1);
	gameboy->bus.WriteWord(address, value);
	// No Flags Effected
}

/* ADD */
void CPU::Opcodes::opcode_add(GBEmu::Gameboy* gameboy, uint8_t value1, uint8_t value2) {
	unsigned int result_long = value1 + value2;
	uint8_t result = static_cast<uint8_t>(result_long);
	gameboy->registers.registers.a = result;

	gameboy->registers.flags_registers.zero = gameboy->registers.registers.a == 0;
	gameboy->registers.flags_registers.subtract = false;
	gameboy->registers.flags_registers.halfcarry = ((value1 & 0x0F) + (value2 & 0x0F)) > 0x0F;
	gameboy->registers.flags_registers.carry = result_long > 0xFF;
	gameboy->registers.SetByteFromFlags();
}

/* ADC */
void CPU::Opcodes::opcode_adc(GBEmu::Gameboy* gameboy, uint8_t value1, uint8_t value2) {
	unsigned int result_long = value1 + value2 + gameboy->registers.flags_registers.carry;
	uint8_t result = static_cast<uint8_t>(result_long);
	gameboy->registers.registers.a = result;

	// Flags Effected
	gameboy->registers.flags_registers.zero = gameboy->registers.registers.a == 0;
	gameboy->registers.flags_registers.subtract = false;
	gameboy->registers.flags_registers.halfcarry = ((value1 & 0x0F) + (value2 & 0x0F) + gameboy->registers.flags_registers.carry) > 0x0F;
	gameboy->registers.flags_registers.carry = result_long > 0xFF;
	gameboy->registers.SetByteFromFlags();
}

/* SUB */
void CPU::Opcodes::opcode_sub(GBEmu::Gameboy* gameboy, uint8_t value1, uint8_t value2) {
	unsigned int result_long = value1 - value2;
	uint8_t result = static_cast<uint8_t>(result_long);
	gameboy->registers.registers.a = result;

	// Flags Effected
	gameboy->registers.flags_registers.zero = gameboy->registers.registers.a == 0;
	gameboy->registers.flags_registers.subtract = true;
	gameboy->registers.flags_registers.halfcarry = ((value1 & 0x0F) - (value2 & 0x0F)) < 0;
	gameboy->registers.flags_registers.carry = (value2 > value1);
	gameboy->registers.SetByteFromFlags();
}

/* SBC */
void CPU::Opcodes::opcode_sbc(GBEmu::Gameboy* gameboy, uint8_t value1, uint8_t value2) {
	unsigned int result_long = value1 - (value2 + gameboy->registers.flags_registers.carry);
	uint8_t result = static_cast<uint8_t>(result_long);
	gameboy->registers.registers.a = result;

	// Flags Effected
	gameboy->registers.flags_registers.zero = gameboy->registers.registers.a == 0;
	gameboy->registers.flags_registers.subtract = true;
	gameboy->registers.flags_registers.halfcarry = ((value1 & 0x0F) - ((value2 & 0x0F) + gameboy->registers.flags_registers.carry)) < 0;
	gameboy->registers.flags_registers.carry = (value2 > value1);
	gameboy->registers.SetByteFromFlags();
}

/* AND */
void CPU::Opcodes::opcode_and(GBEmu::Gameboy* gameboy, uint8_t value1, uint8_t value2) {
	uint8_t result = (value1 & value2);
	gameboy->registers.registers.a = result;

	// Flags Effected
	gameboy->registers.flags_registers.subtract = gameboy->registers.registers.a == 0;
	gameboy->registers.flags_registers.subtract = false;
	gameboy->registers.flags_registers.halfcarry = true;
	gameboy->registers.flags_registers.carry = false;
	gameboy->registers.SetByteFromFlags();
}

/* XOR */
void CPU::Opcodes::opcode_xor(GBEmu::Gameboy* gameboy, uint8_t value1, uint8_t value2) {
	uint8_t result = (value1 ^ value2);
	gameboy->registers.registers.a = result;

	// Flags Effected
	gameboy->registers.flags_registers.subtract = gameboy->registers.registers.a == 0;
	gameboy->registers.flags_registers.subtract = false;
	gameboy->registers.flags_registers.halfcarry = false;
	gameboy->registers.flags_registers.carry = false;
	gameboy->registers.SetByteFromFlags();
}

/* OR */
void CPU::Opcodes::opcode_or(GBEmu::Gameboy* gameboy, uint8_t value1, uint8_t value2) {
	uint8_t result = (value1 | value2);
	gameboy->registers.registers.a = result;

	// Flags Effected
	gameboy->registers.flags_registers.subtract = gameboy->registers.registers.a == 0;
	gameboy->registers.flags_registers.subtract = false;
	gameboy->registers.flags_registers.halfcarry = false;
	gameboy->registers.flags_registers.carry = false;
	gameboy->registers.SetByteFromFlags();
}

/* JP */
void CPU::Opcodes::opcode_jp(GBEmu::Gameboy* gameboy, uint16_t address) {
	gameboy->cpu.pc = address;
	// No Flags Effected
}

void CPU::Opcodes::opcode_jp_mem(GBEmu::Gameboy* gameboy) {
	opcode_jp(gameboy, gameboy->bus.ReadWord(gameboy->cpu.pc + 1));
}

void CPU::Opcodes::opcode_jp_reg(GBEmu::Gameboy* gameboy) {
	// TODO: Implement
}

/* RST */
void CPU::Opcodes::opcode_rst(GBEmu::Gameboy* gameboy, uint16_t address) {
	gameboy->cpu.Push(gameboy, gameboy->cpu.pc);
	gameboy->cpu.pc = address;
}