#include <iostream>
#include <stdio.h>
#include "Registers.h"
#include "Constants.h"

using namespace std;
using namespace GBEmu;

void CPU::Registers::SetAF(uint16_t af) {
	registers.a = (uint8_t)((af & 0xFF00) >> 8);
	registers.f = (uint8_t)(af & 0x00FF);
}
uint16_t CPU::Registers::GetAF() {
	uint16_t af = ((uint16_t)registers.a << 8) | (uint16_t)registers.f;
	return af;
}

void CPU::Registers::SetBC(uint16_t bc) {
	registers.b = (uint8_t)((bc & 0xFF00) >> 8);
	registers.c = (uint8_t)(bc & 0x00FF);
}
uint16_t CPU::Registers::GetBC() {
	uint16_t bc = ((uint16_t)registers.b << 8) | (uint16_t)registers.c;
	return bc;
}

void CPU::Registers::SetDE(uint16_t de) {
	registers.d = (uint8_t)((de & 0xFF00) >> 8);
	registers.e = (uint8_t)(de & 0x00FF);
}
uint16_t CPU::Registers::GetDE() {
	uint16_t de = ((uint16_t)registers.d << 8) | (uint16_t)registers.e;
	return de;
}

void CPU::Registers::SetHL(uint16_t hl) {
	registers.h = (uint8_t)((hl & 0xFF00) >> 8);
	registers.l = (uint8_t)(hl & 0x00FF);
}

uint16_t CPU::Registers::GetHL() {
	uint16_t hl = ((uint16_t)registers.h << 8) | (uint16_t)registers.l;
	return hl;
}

void CPU::Registers::SetByteFromFlags() {
	uint8_t byte = (uint8_t)flags_registers.zero << 7;
	byte |= (uint8_t)flags_registers.subtract << 6;
	byte |= (uint8_t)flags_registers.halfcarry << 5;
	byte |= (uint8_t)flags_registers.carry << 4;
	registers.f = byte;
}