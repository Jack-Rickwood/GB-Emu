#include <iostream>
#include <stdio.h>
#include "CPU.h"
#include "Gameboy.h"

using namespace std;
using namespace GBEmu;

int CPU::CPU::HandleInstruction(Gameboy* gameboy) {
	uint8_t instruction_byte = gameboy->bus.ReadByte(gameboy, pc);
	uint8_t cb_instruction_byte = gameboy->bus.ReadByte(gameboy, pc + 1);
	if (instruction_byte == 0xCB) {
		return ExecuteCBInstruction(gameboy, cb_instruction_byte) * 4;
	}
	else {
		return ExecuteInstruction(gameboy, instruction_byte) * 4;
	}
}

int CPU::CPU::ExecuteInstruction(Gameboy* gameboy, uint8_t instruction) {
	if (gameboy->debug) {
		printf("[Debug] Program counter is at 0x%X\n", pc);
		printf("[Debug] Info:\n\tRegisters:\n\t\tA: 0x%X\n\t\tB: 0x%X\n\t\tC: 0x%X\n\t\tD: 0x%X\n\t\tE: 0x%X\n\t\tF: 0x%X\n\t\tH: 0x%X\n\t\tL: 0x%X\n\t\tAF: 0x%X\n\t\tBC: 0x%X\n\t\tDE: 0x%X\n\t\tHL: 0x%X\n\tFlags Registers:\n\t\tZero: 0x%X\n\t\tSubtract: 0x%X\n\t\tHalf-Carry: 0x%X\n\t\tCarry: 0x%X\n", gameboy->registers.registers.a, gameboy->registers.registers.b, gameboy->registers.registers.c, gameboy->registers.registers.d, gameboy->registers.registers.e, gameboy->registers.registers.f, gameboy->registers.registers.h, gameboy->registers.registers.l, gameboy->registers.GetAF(), gameboy->registers.GetBC(), gameboy->registers.GetDE(), gameboy->registers.GetHL(), gameboy->registers.flags_registers.zero, gameboy->registers.flags_registers.subtract, gameboy->registers.flags_registers.halfcarry, gameboy->registers.flags_registers.carry);
	}
	uint8_t value;
	switch (instruction) {
	case 0x00:
		if (gameboy->debug) {
			printf("[Debug] Instruction NOP called\n");
		}
		pc += 1;
		break;
	case 0x01:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called on BC(Register)\n");
		}
		gameboy->opcodes.opcode_ld_bc(gameboy);
		pc += 3;
		break;
	case 0x02:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: (BC), A(Register)\n");
		}
		gameboy->opcodes.opcode_ld_bc_deref(gameboy, gameboy->registers.registers.a);
		pc += 1;
		break;
	case 0x03:
		if (gameboy->debug) {
			printf("[Debug] Instruction INC called on BC(Register)\n");
		}
		gameboy->registers.SetBC(gameboy->registers.GetBC() + 1);
		pc += 1;
		break;
	case 0x04:
		if (gameboy->debug) {
			printf("[Debug] Instruction INC called on B(Register)\n");
		}
		value = gameboy->registers.registers.b + 1;
		gameboy->registers.registers.b = value;
		gameboy->opcodes.opcode_inc_set_flags(gameboy, value);
		pc += 1;
		break;
	case 0x05:
		if (gameboy->debug) {
			printf("[Debug] Instruction DEC called on B(Register)\n");
		}
		value = gameboy->registers.registers.b - 1;
		gameboy->registers.registers.b = value;
		gameboy->opcodes.opcode_dec_set_flags(gameboy, value);
		pc += 1;
		break;
	case 0x06:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called on B(Register)\n");
		}
		gameboy->opcodes.opcode_ld_b(gameboy, gameboy->bus.ReadByte(gameboy, pc + 1));
		pc += 2;
		break;
	case 0x08:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called on B(Register)\n");
		}
		gameboy->opcodes.opcode_ld_mem(gameboy, sp);
		pc += 3;
		break;
	case 0x0C:
		if (gameboy->debug) {
			printf("[Debug] Instruction INC called on C(Register)\n");
		}
		value = gameboy->registers.registers.c + 1;
		gameboy->registers.registers.c = value;
		gameboy->opcodes.opcode_inc_set_flags(gameboy, value);
		pc += 1;
		break;
	case 0x0D:
		if (gameboy->debug) {
			printf("[Debug] Instruction DEC called on C(Register)\n");
		}
		value = gameboy->registers.registers.c - 1;
		gameboy->registers.registers.c = value;
		gameboy->opcodes.opcode_dec_set_flags(gameboy, value);
		pc += 1;
		break;
	case 0x0E:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called on C(Register)\n");
		}
		gameboy->opcodes.opcode_ld_c(gameboy, gameboy->bus.ReadByte(gameboy, pc + 1));
		pc += 2;
		break;
	case 0x11:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called on DE(Register)\n");
		}
		gameboy->opcodes.opcode_ld_de(gameboy);
		pc += 3;
		break;
	case 0x12:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: (DE), A(Register)\n");
		}
		gameboy->opcodes.opcode_ld_de_deref(gameboy, gameboy->registers.registers.a);
		pc += 1;
		break;
	case 0x13:
		if (gameboy->debug) {
			printf("[Debug] Instruction INC called on DE(Register)\n");
		}
		gameboy->registers.SetDE(gameboy->registers.GetDE() + 1);
		pc += 1;
		break;
	case 0x14:
		if (gameboy->debug) {
			printf("[Debug] Instruction INC called on D(Register)\n");
		}
		value = gameboy->registers.registers.d + 1;
		gameboy->registers.registers.d = value;
		gameboy->opcodes.opcode_inc_set_flags(gameboy, value);
		pc += 1;
		break;
	case 0x15:
		if (gameboy->debug) {
			printf("[Debug] Instruction DEC called on D(Register)\n");
		}
		value = gameboy->registers.registers.d - 1;
		gameboy->registers.registers.d = value;
		gameboy->opcodes.opcode_inc_set_flags(gameboy, value);
		pc += 1;
		break;
	case 0x16:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called on D(Register)\n");
		}
		gameboy->opcodes.opcode_ld_d(gameboy, gameboy->bus.ReadByte(gameboy, pc + 1));
		pc += 2;
		break;
	case 0x1C:
		if (gameboy->debug) {
			printf("[Debug] Instruction INC called on E(Register)\n");
		}
		value = gameboy->registers.registers.e + 1;
		gameboy->registers.registers.e = value;
		gameboy->opcodes.opcode_inc_set_flags(gameboy, value);
		pc += 1;
		break;
	case 0x1D:
		if (gameboy->debug) {
			printf("[Debug] Instruction DEC called on E(Register)\n");
		}
		value = gameboy->registers.registers.e - 1;
		gameboy->registers.registers.e = value;
		gameboy->opcodes.opcode_dec_set_flags(gameboy, value);
		pc += 1;
		break;
	case 0x20:
		if (gameboy->debug) {
			printf("[Debug] Instruction JR called with condition !Zero\n");
		}
		if (!gameboy->registers.flags_registers.zero) {
			pc += (int8_t)gameboy->bus.ReadByte(gameboy, pc + 1) + 2;
			printf("\n");
			return 3;
		}
		else {
			pc += 2;
		}
		break;
	case 0x21:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called on HL(Register)\n");
		}
		gameboy->opcodes.opcode_ld_hl(gameboy);
		pc += 3;
		break;
	case 0x23:
		if (gameboy->debug) {
			printf("[Debug] Instruction INC called on HL(Register)\n");
		}
		gameboy->registers.SetHL(gameboy->registers.GetHL() + 1);
		pc += 1;
		break;
	case 0x24:
		if (gameboy->debug) {
			printf("[Debug] Instruction INC called on H(Register)\n");
		}
		value = gameboy->registers.registers.h + 1;
		gameboy->registers.registers.h = value;
		gameboy->opcodes.opcode_inc_set_flags(gameboy, value);
		pc += 1;
		break;
	case 0x25:
		if (gameboy->debug) {
			printf("[Debug] Instruction DEC called on H(Register)\n");
		}
		value = gameboy->registers.registers.h - 1;
		gameboy->registers.registers.h = value;
		gameboy->opcodes.opcode_dec_set_flags(gameboy, value);
		pc += 1;
		break;
	case 0x26:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called on H(Register)\n");
		}
		gameboy->opcodes.opcode_ld_h(gameboy, gameboy->bus.ReadByte(gameboy, pc + 1));
		pc += 2;
		break;
	case 0x2C:
		if (gameboy->debug) {
			printf("[Debug] Instruction INC called on L(Register)\n");
		}
		value = gameboy->registers.registers.l + 1;
		gameboy->registers.registers.l = value;
		gameboy->opcodes.opcode_inc_set_flags(gameboy, value);
		pc += 1;
		break;
	case 0x2D:
		if (gameboy->debug) {
			printf("[Debug] Instruction DEC called on L(Register)\n");
		}
		value = gameboy->registers.registers.l - 1;
		gameboy->registers.registers.l = value;
		gameboy->opcodes.opcode_dec_set_flags(gameboy, value);
		pc += 1;
		break;
	case 0x2F:
		if (gameboy->debug) {
			printf("[Debug] Instruction CPL called\n");
		}
		gameboy->opcodes.opcode_cpl(gameboy);
		pc += 1;
		break;
	case 0x30:
		if (gameboy->debug) {
			printf("[Debug] Instruction JR called with condition !Carry\n");
		}
		if (!gameboy->registers.flags_registers.carry) {
			pc += (int8_t)gameboy->bus.ReadByte(gameboy, pc + 1) + 2;
			printf("\n");
			return 3;
		}
		else {
			pc += 2;
		}
		break;
	case 0x31:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called on SP\n");
		}
		gameboy->opcodes.opcode_ld_sp(gameboy);
		pc += 3;
		break;
	case 0x32:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: (HL-), A(Register)\n");
		}
		gameboy->opcodes.opcode_ld_hl_deref(gameboy, gameboy->registers.registers.a);
		gameboy->registers.SetHL(gameboy->registers.GetHL() - 1);
		pc += 1;
		break;
	case 0x33:
		if (gameboy->debug) {
			printf("[Debug] Instruction INC called on SP\n");
		}
		sp++;
		pc += 1;
		break;
	case 0x34:
		if (gameboy->debug) {
			printf("[Debug] Instruction INC called on (HL)\n");
		}
		gameboy->opcodes.opcode_inc_hl_deref(gameboy);
		pc += 1;
		break;
	case 0x35:
		if (gameboy->debug) {
			printf("[Debug] Instruction DEC called on (HL)\n");
		}
		gameboy->opcodes.opcode_dec_hl_deref(gameboy);
		pc += 1;
		break;
	case 0x36:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called on (HL)\n");
		}
		gameboy->opcodes.opcode_ld_hl_deref(gameboy, gameboy->bus.ReadByte(gameboy, pc + 1));
		pc += 2;
		break;
	case 0x3C:
		if (gameboy->debug) {
			printf("[Debug] Instruction INC called on A(Register)\n");
		}
		value = gameboy->registers.registers.a + 1;
		gameboy->registers.registers.a = value;
		gameboy->opcodes.opcode_inc_set_flags(gameboy, value);
		pc += 1;
		break;
	case 0x3D:
		if (gameboy->debug) {
			printf("[Debug] Instruction DEC called on A(Register)\n");
		}
		value = gameboy->registers.registers.a - 1;
		gameboy->registers.registers.a = value;
		gameboy->opcodes.opcode_dec_set_flags(gameboy, value);
		pc += 1;
		break;
	case 0x40:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: B(Register), B(Register)\n");
		}
		gameboy->opcodes.opcode_ld_b(gameboy, gameboy->registers.registers.b);
		pc += 1;
		break;
	case 0x41:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: B(Register), C(Register)\n");
		}
		gameboy->opcodes.opcode_ld_b(gameboy, gameboy->registers.registers.c);
		pc += 1;
		break;
	case 0x42:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: B(Register), D(Register)\n");
		}
		gameboy->opcodes.opcode_ld_b(gameboy, gameboy->registers.registers.d);
		pc += 1;
		break;
	case 0x43:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: B(Register), E(Register)\n");
		}
		gameboy->opcodes.opcode_ld_b(gameboy, gameboy->registers.registers.e);
		pc += 1;
		break;
	case 0x44:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: B(Register), H(Register)\n");
		}
		gameboy->opcodes.opcode_ld_b(gameboy, gameboy->registers.registers.h);
		pc += 1;
		break;
	case 0x45:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: B(Register), L(Register)\n");
		}
		gameboy->opcodes.opcode_ld_b(gameboy, gameboy->registers.registers.l);
		pc += 1;
		break;
	case 0x46:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: B(Register), (HL)\n");
		}
		gameboy->opcodes.opcode_ld_b(gameboy, gameboy->bus.ReadByte(gameboy, gameboy->registers.GetHL()));
		pc += 1;
		break;
	case 0x47:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: B(Register), A(Register)\n");
		}
		gameboy->opcodes.opcode_ld_b(gameboy, gameboy->registers.registers.a);
		pc += 1;
		break;
	case 0x48:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: C(Register), B(Register)\n");
		}
		gameboy->opcodes.opcode_ld_c(gameboy, gameboy->registers.registers.b);
		pc += 1;
		break;
	case 0x49:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: C(Register), C(Register)\n");
		}
		gameboy->opcodes.opcode_ld_c(gameboy, gameboy->registers.registers.c);
		pc += 1;
		break;
	case 0x4A:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: C(Register), D(Register)\n");
		}
		gameboy->opcodes.opcode_ld_c(gameboy, gameboy->registers.registers.d);
		pc += 1;
		break;
	case 0x4B:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: C(Register), E(Register)\n");
		}
		gameboy->opcodes.opcode_ld_c(gameboy, gameboy->registers.registers.e);
		pc += 1;
		break;
	case 0x4C:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: C(Register), H(Register)\n");
		}
		gameboy->opcodes.opcode_ld_c(gameboy, gameboy->registers.registers.h);
		pc += 1;
		break;
	case 0x4D:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: C(Register), L(Register)\n");
		}
		gameboy->opcodes.opcode_ld_c(gameboy, gameboy->registers.registers.l);
		pc += 1;
		break;
	case 0x4E:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: C(Register), (HL)\n");
		}
		gameboy->opcodes.opcode_ld_c(gameboy, gameboy->bus.ReadByte(gameboy, gameboy->registers.GetHL()));
		pc += 1;
		break;
	case 0x4F:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: C(Register), A(Register)\n");
		}
		gameboy->opcodes.opcode_ld_c(gameboy, gameboy->registers.registers.a);
		pc += 1;
		break;
	case 0x50:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: D(Register), B(Register)\n");
		}
		gameboy->opcodes.opcode_ld_d(gameboy, gameboy->registers.registers.b);
		pc += 1;
		break;
	case 0x51:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: D(Register), C(Register)\n");
		}
		gameboy->opcodes.opcode_ld_d(gameboy, gameboy->registers.registers.c);
		pc += 1;
		break;
	case 0x52:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: D(Register), D(Register)\n");
		}
		gameboy->opcodes.opcode_ld_d(gameboy, gameboy->registers.registers.d);
		pc += 1;
		break;
	case 0x53:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: D(Register), E(Register)\n");
		}
		gameboy->opcodes.opcode_ld_d(gameboy, gameboy->registers.registers.e);
		pc += 1;
		break;
	case 0x54:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: D(Register), H(Register)\n");
		}
		gameboy->opcodes.opcode_ld_d(gameboy, gameboy->registers.registers.h);
		pc += 1;
		break;
	case 0x55:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: D(Register), L(Register)\n");
		}
		gameboy->opcodes.opcode_ld_d(gameboy, gameboy->registers.registers.l);
		pc += 1;
		break;
	case 0x56:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: D(Register), (HL)\n");
		}
		gameboy->opcodes.opcode_ld_d(gameboy, gameboy->bus.ReadByte(gameboy, gameboy->registers.GetHL()));
		pc += 1;
		break;
	case 0x57:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: D(Register), A(Register)\n");
		}
		gameboy->opcodes.opcode_ld_d(gameboy, gameboy->registers.registers.a);
		pc += 1;
		break;
	case 0x58:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: E(Register), B(Register)\n");
		}
		gameboy->opcodes.opcode_ld_e(gameboy, gameboy->registers.registers.b);
		pc += 1;
		break;
	case 0x59:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: E(Register), C(Register)\n");
		}
		gameboy->opcodes.opcode_ld_e(gameboy, gameboy->registers.registers.c);
		pc += 1;
		break;
	case 0x5A:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: E(Register), D(Register)\n");
		}
		gameboy->opcodes.opcode_ld_e(gameboy, gameboy->registers.registers.d);
		pc += 1;
		break;
	case 0x5B:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: E(Register), E(Register)\n");
		}
		gameboy->opcodes.opcode_ld_e(gameboy, gameboy->registers.registers.e);
		pc += 1;
		break;
	case 0x5C:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: E(Register), H(Register)\n");
		}
		gameboy->opcodes.opcode_ld_e(gameboy, gameboy->registers.registers.h);
		pc += 1;
		break;
	case 0x5D:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: E(Register), L(Register)\n");
		}
		gameboy->opcodes.opcode_ld_e(gameboy, gameboy->registers.registers.l);
		pc += 1;
		break;
	case 0x5E:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: E(Register), (HL)\n");
		}
		gameboy->opcodes.opcode_ld_e(gameboy, gameboy->bus.ReadByte(gameboy, gameboy->registers.GetHL()));
		pc += 1;
		break;
	case 0x5F:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: E(Register), A(Register)\n");
		}
		gameboy->opcodes.opcode_ld_e(gameboy, gameboy->registers.registers.a);
		pc += 1;
		break;
	case 0x60:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: H(Register), B(Register)\n");
		}
		gameboy->opcodes.opcode_ld_h(gameboy, gameboy->registers.registers.b);
		pc += 1;
		break;
	case 0x61:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: H(Register), C(Register)\n");
		}
		gameboy->opcodes.opcode_ld_h(gameboy, gameboy->registers.registers.c);
		pc += 1;
		break;
	case 0x62:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: H(Register), D(Register)\n");
		}
		gameboy->opcodes.opcode_ld_h(gameboy, gameboy->registers.registers.d);
		pc += 1;
		break;
	case 0x63:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: H(Register), E(Register)\n");
		}
		gameboy->opcodes.opcode_ld_h(gameboy, gameboy->registers.registers.e);
		pc += 1;
		break;
	case 0x64:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: H(Register), H(Register)\n");
		}
		gameboy->opcodes.opcode_ld_h(gameboy, gameboy->registers.registers.h);
		pc += 1;
		break;
	case 0x65:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: H(Register), L(Register)\n");
		}
		gameboy->opcodes.opcode_ld_h(gameboy, gameboy->registers.registers.l);
		pc += 1;
		break;
	case 0x66:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: H(Register), (HL)\n");
		}
		gameboy->opcodes.opcode_ld_h(gameboy, gameboy->bus.ReadByte(gameboy, gameboy->registers.GetHL()));
		pc += 1;
		break;
	case 0x67:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: H(Register), A(Register)\n");
		}
		gameboy->opcodes.opcode_ld_h(gameboy, gameboy->registers.registers.a);
		pc += 1;
		break;
	case 0x68:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: L(Register), B(Register)\n");
		}
		gameboy->opcodes.opcode_ld_l(gameboy, gameboy->registers.registers.b);
		pc += 1;
		break;
	case 0x69:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: L(Register), C(Register)\n");
		}
		gameboy->opcodes.opcode_ld_l(gameboy, gameboy->registers.registers.c);
		pc += 1;
		break;
	case 0x6A:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: L(Register), D(Register)\n");
		}
		gameboy->opcodes.opcode_ld_l(gameboy, gameboy->registers.registers.d);
		pc += 1;
		break;
	case 0x6B:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: L(Register), E(Register)\n");
		}
		gameboy->opcodes.opcode_ld_l(gameboy, gameboy->registers.registers.e);
		pc += 1;
		break;
	case 0x6C:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: L(Register), H(Register)\n");
		}
		gameboy->opcodes.opcode_ld_l(gameboy, gameboy->registers.registers.h);
		pc += 1;
		break;
	case 0x6D:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: L(Register), L(Register)\n");
		}
		gameboy->opcodes.opcode_ld_l(gameboy, gameboy->registers.registers.l);
		pc += 1;
		break;
	case 0x6E:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: L(Register), (HL)\n");
		}
		gameboy->opcodes.opcode_ld_l(gameboy, gameboy->bus.ReadByte(gameboy, gameboy->registers.GetHL()));
		pc += 1;
		break;
	case 0x6F:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: L(Register), A(Register)\n");
		}
		gameboy->opcodes.opcode_ld_l(gameboy, gameboy->registers.registers.a);
		pc += 1;
		break;
	case 0x70:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: (HL), B(Register)\n");
		}
		gameboy->opcodes.opcode_ld_hl_deref(gameboy, gameboy->registers.registers.b);
		pc += 1;
		break;
	case 0x71:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: (HL), C(Register)\n");
		}
		gameboy->opcodes.opcode_ld_hl_deref(gameboy, gameboy->registers.registers.c);
		pc += 1;
		break;
	case 0x72:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: (HL), D(Register)\n");
		}
		gameboy->opcodes.opcode_ld_hl_deref(gameboy, gameboy->registers.registers.d);
		pc += 1;
		break;
	case 0x73:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: (HL), E(Register)\n");
		}
		gameboy->opcodes.opcode_ld_hl_deref(gameboy, gameboy->registers.registers.e);
		pc += 1;
		break;
	case 0x74:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: (HL), H(Register)\n");
		}
		gameboy->opcodes.opcode_ld_hl_deref(gameboy, gameboy->registers.registers.h);
		pc += 1;
		break;
	case 0x75:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: (HL), L(Register)\n");
		}
		gameboy->opcodes.opcode_ld_hl_deref(gameboy, gameboy->registers.registers.l);
		pc += 1;
		break;
	case 0x77:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: (HL), A(Register)\n");
		}
		gameboy->opcodes.opcode_ld_hl_deref(gameboy, gameboy->registers.registers.a);
		pc += 1;
		break;
	case 0x78:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: A(Register), B(Register)\n");
		}
		gameboy->opcodes.opcode_ld_a(gameboy, gameboy->registers.registers.b);
		pc += 1;
		break;
	case 0x79:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: A(Register), C(Register)\n");
		}
		gameboy->opcodes.opcode_ld_a(gameboy, gameboy->registers.registers.c);
		pc += 1;
		break;
	case 0x7A:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: A(Register), D(Register)\n");
		}
		gameboy->opcodes.opcode_ld_a(gameboy, gameboy->registers.registers.d);
		pc += 1;
		break;
	case 0x7B:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: A(Register), E(Register)\n");
		}
		gameboy->opcodes.opcode_ld_a(gameboy, gameboy->registers.registers.e);
		pc += 1;
		break;
	case 0x7C:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: A(Register), H(Register)\n");
		}
		gameboy->opcodes.opcode_ld_a(gameboy, gameboy->registers.registers.h);
		pc += 1;
		break;
	case 0x7D:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: A(Register), L(Register)\n");
		}
		gameboy->opcodes.opcode_ld_a(gameboy, gameboy->registers.registers.l);
		pc += 1;
		break;
	case 0x7E:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: A(Register), (HL)\n");
		}
		gameboy->opcodes.opcode_ld_a(gameboy, gameboy->bus.ReadByte(gameboy, gameboy->registers.GetHL()));
		pc += 1;
		break;
	case 0x7F:
		if (gameboy->debug) {
			printf("[Debug] Instruction LD called with arguments: A(Register), A(Register)\n");
		}
		gameboy->opcodes.opcode_ld_a(gameboy, gameboy->registers.registers.a);
		pc += 1;
		break;
	case 0x80:
		if (gameboy->debug) {
			printf("[Debug] Instruction ADD called with arguments: A(Register), B(Register)\n");
		}
		gameboy->opcodes.opcode_add(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.b);
		pc += 1;
		break;
	case 0x81:
		if (gameboy->debug) {
			printf("[Debug] Instruction ADD called with arguments: A(Register), C(Register)\n");
		}
		gameboy->opcodes.opcode_add(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.c);
		pc += 1;
		break;
	case 0x82:
		if (gameboy->debug) {
			printf("[Debug] Instruction ADD called with arguments: A(Register), D(Register)\n");
		}
		gameboy->opcodes.opcode_add(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.d);
		pc += 1;
		break;
	case 0x83:
		if (gameboy->debug) {
			printf("[Debug] Instruction ADD called with arguments: A(Register), E(Register)\n");
		}
		gameboy->opcodes.opcode_add(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.e);
		pc += 1;
		break;
	case 0x84:
		if (gameboy->debug) {
			printf("[Debug] Instruction ADD called with arguments: A(Register), H(Register)\n");
		}
		gameboy->opcodes.opcode_add(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.h);
		pc += 1;
		break;
	case 0x85:
		if (gameboy->debug) {
			printf("[Debug] Instruction ADD called with arguments: A(Register), L(Register)\n");
		}
		gameboy->opcodes.opcode_add(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.l);
		pc += 1;
		break;
	case 0x86:
		if (gameboy->debug) {
			printf("[Debug] Instruction ADD called with arguments: A(Register), (HL)\n");
		}
		gameboy->opcodes.opcode_add(gameboy, gameboy->registers.registers.a, gameboy->bus.ReadByte(gameboy, gameboy->registers.GetHL()));
		pc += 1;
		break;
	case 0x87:
		if (gameboy->debug) {
			printf("[Debug] Instruction ADD called with arguments: A(Register), A(Register)\n");
		}
		gameboy->opcodes.opcode_add(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.a);
		pc += 1;
		break;
	case 0x88:
		if (gameboy->debug) {
			printf("[Debug] Instruction ADC called with arguments: A(Register), B(Register)\n");
		}
		gameboy->opcodes.opcode_adc(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.b);
		pc += 1;
		break;
	case 0x89:
		if (gameboy->debug) {
			printf("[Debug] Instruction ADC called with arguments: A(Register), C(Register)\n");
		}
		gameboy->opcodes.opcode_adc(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.c);
		pc += 1;
		break;
	case 0x8A:
		if (gameboy->debug) {
			printf("[Debug] Instruction ADC called with arguments: A(Register), D(Register)\n");
		}
		gameboy->opcodes.opcode_adc(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.d);
		pc += 1;
		break;
	case 0x8B:
		if (gameboy->debug) {
			printf("[Debug] Instruction ADC called with arguments: A(Register), E(Register)\n");
		}
		gameboy->opcodes.opcode_adc(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.e);
		pc += 1;
		break;
	case 0x8C:
		if (gameboy->debug) {
			printf("[Debug] Instruction ADC called with arguments: A(Register), H(Register)\n");
		}
		gameboy->opcodes.opcode_adc(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.h);
		pc += 1;
		break;
	case 0x8D:
		if (gameboy->debug) {
			printf("[Debug] Instruction ADC called with arguments: A(Register), L(Register)\n");
		}
		gameboy->opcodes.opcode_adc(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.l);
		pc += 1;
		break;
	case 0x8E:
		if (gameboy->debug) {
			printf("[Debug] Instruction ADC called with arguments: A(Register), (HL)\n");
		}
		gameboy->opcodes.opcode_adc(gameboy, gameboy->registers.registers.a, gameboy->bus.ReadByte(gameboy, gameboy->registers.GetHL()));
		pc += 1;
		break;
	case 0x8F:
		if (gameboy->debug) {
			printf("[Debug] Instruction ADC called with arguments: A(Register), A(Register)\n");
		}
		gameboy->opcodes.opcode_adc(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.a);
		pc += 1;
		break;
	case 0x90:
		if (gameboy->debug) {
			printf("[Debug] Instruction SUB called with arguments: A(Register), B(Register)\n");
		}
		gameboy->opcodes.opcode_sub(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.b);
		pc += 1;
		break;
	case 0x91:
		if (gameboy->debug) {
			printf("[Debug] Instruction SUB called with arguments: A(Register), C(Register)\n");
		}
		gameboy->opcodes.opcode_sub(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.c);
		pc += 1;
		break;
	case 0x92:
		if (gameboy->debug) {
			printf("[Debug] Instruction SUB called with arguments: A(Register), D(Register)\n");
		}
		gameboy->opcodes.opcode_sub(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.d);
		pc += 1;
		break;
	case 0x93:
		if (gameboy->debug) {
			printf("[Debug] Instruction SUB called with arguments: A(Register), E(Register)\n");
		}
		gameboy->opcodes.opcode_sub(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.e);
		pc += 1;
		break;
	case 0x94:
		if (gameboy->debug) {
			printf("[Debug] Instruction SUB called with arguments: A(Register), H(Register)\n");
		}
		gameboy->opcodes.opcode_sub(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.h);
		pc += 1;
		break;
	case 0x95:
		if (gameboy->debug) {
			printf("[Debug] Instruction SUB called with arguments: A(Register), L(Register)\n");
		}
		gameboy->opcodes.opcode_sub(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.l);
		pc += 1;
		break;
	case 0x96:
		if (gameboy->debug) {
			printf("[Debug] Instruction SUB called with arguments: A(Register), (HL)\n");
		}
		gameboy->opcodes.opcode_sub(gameboy, gameboy->registers.registers.a, gameboy->bus.ReadByte(gameboy, gameboy->registers.GetHL()));
		pc += 1;
		break;
	case 0x97:
		if (gameboy->debug) {
			printf("[Debug] Instruction SUB called with arguments: A(Register), A(Register)\n");
		}
		gameboy->opcodes.opcode_sub(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.a);
		pc += 1;
		break;
	case 0x98:
		if (gameboy->debug) {
			printf("[Debug] Instruction SBC called with arguments: A(Register), B(Register)\n");
		}
		gameboy->opcodes.opcode_sbc(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.b);
		pc += 1;
		break;
	case 0x99:
		if (gameboy->debug) {
			printf("[Debug] Instruction SBC called with arguments: A(Register), C(Register)\n");
		}
		gameboy->opcodes.opcode_sbc(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.c);
		pc += 1;
		break;
	case 0x9A:
		if (gameboy->debug) {
			printf("[Debug] Instruction SBC called with arguments: A(Register), D(Register)\n");
		}
		gameboy->opcodes.opcode_sbc(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.d);
		pc += 1;
		break;
	case 0x9B:
		if (gameboy->debug) {
			printf("[Debug] Instruction SBC called with arguments: A(Register), E(Register)\n");
		}
		gameboy->opcodes.opcode_sbc(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.e);
		pc += 1;
		break;
	case 0x9C:
		if (gameboy->debug) {
			printf("[Debug] Instruction SBC called with arguments: A(Register), H(Register)\n");
		}
		gameboy->opcodes.opcode_sbc(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.h);
		pc += 1;
		break;
	case 0x9D:
		if (gameboy->debug) {
			printf("[Debug] Instruction SBC called with arguments: A(Register), L(Register)\n");
		}
		gameboy->opcodes.opcode_sbc(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.l);
		pc += 1;
		break;
	case 0x9E:
		if (gameboy->debug) {
			printf("[Debug] Instruction SBC called with arguments: A(Register), (HL)\n");
		}
		gameboy->opcodes.opcode_sbc(gameboy, gameboy->registers.registers.a, gameboy->bus.ReadByte(gameboy, gameboy->registers.GetHL()));
		pc += 1;
		break;
	case 0x9F:
		if (gameboy->debug) {
			printf("[Debug] Instruction SBC called with arguments: A(Register), A(Register)\n");
		}
		gameboy->opcodes.opcode_sbc(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.a);
		pc += 1;
		break;
	case 0xA0:
		if (gameboy->debug) {
			printf("[Debug] Instruction AND called with arguments: A(Register), B(Register)\n");
		}
		gameboy->opcodes.opcode_and(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.b);
		pc += 1;
		break;
	case 0xA1:
		if (gameboy->debug) {
			printf("[Debug] Instruction AND called with arguments: A(Register), C(Register)\n");
		}
		gameboy->opcodes.opcode_and(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.c);
		pc += 1;
		break;
	case 0xA2:
		if (gameboy->debug) {
			printf("[Debug] Instruction AND called with arguments: A(Register), D(Register)\n");
		}
		gameboy->opcodes.opcode_and(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.d);
		pc += 1;
		break;
	case 0xA3:
		if (gameboy->debug) {
			printf("[Debug] Instruction AND called with arguments: A(Register), E(Register)\n");
		}
		gameboy->opcodes.opcode_and(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.e);
		pc += 1;
		break;
	case 0xA4:
		if (gameboy->debug) {
			printf("[Debug] Instruction AND called with arguments: A(Register), H(Register)\n");
		}
		gameboy->opcodes.opcode_and(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.h);
		pc += 1;
		break;
	case 0xA5:
		if (gameboy->debug) {
			printf("[Debug] Instruction AND called with arguments: A(Register), L(Register)\n");
		}
		gameboy->opcodes.opcode_and(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.l);
		pc += 1;
		break;
	case 0xA6:
		if (gameboy->debug) {
			printf("[Debug] Instruction AND called with arguments: A(Register), (HL)\n");
		}
		gameboy->opcodes.opcode_and(gameboy, gameboy->registers.registers.a, gameboy->bus.ReadByte(gameboy, gameboy->registers.GetHL()));
		pc += 1;
		break;
	case 0xA7:
		if (gameboy->debug) {
			printf("[Debug] Instruction AND called with arguments: A(Register), A(Register)\n");
		}
		gameboy->opcodes.opcode_and(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.a);
		pc += 1;
		break;
	case 0xA8:
		if (gameboy->debug) {
			printf("[Debug] Instruction XOR called with arguments: A(Register), B(Register)\n");
		}
		gameboy->opcodes.opcode_xor(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.b);
		pc += 1;
		break;
	case 0xA9:
		if (gameboy->debug) {
			printf("[Debug] Instruction XOR called with arguments: A(Register), C(Register)\n");
		}
		gameboy->opcodes.opcode_xor(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.c);
		pc += 1;
		break;
	case 0xAA:
		if (gameboy->debug) {
			printf("[Debug] Instruction XOR called with arguments: A(Register), D(Register)\n");
		}
		gameboy->opcodes.opcode_xor(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.d);
		pc += 1;
		break;
	case 0xAB:
		if (gameboy->debug) {
			printf("[Debug] Instruction XOR called with arguments: A(Register), E(Register)\n");
		}
		gameboy->opcodes.opcode_xor(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.e);
		pc += 1;
		break;
	case 0xAC:
		if (gameboy->debug) {
			printf("[Debug] Instruction XOR called with arguments: A(Register), H(Register)\n");
		}
		gameboy->opcodes.opcode_xor(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.h);
		pc += 1;
		break;
	case 0xAD:
		if (gameboy->debug) {
			printf("[Debug] Instruction XOR called with arguments: A(Register), L(Register)\n");
		}
		gameboy->opcodes.opcode_xor(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.l);
		pc += 1;
		break;
	case 0xAE:
		if (gameboy->debug) {
			printf("[Debug] Instruction XOR called with arguments: A(Register), (HL)\n");
		}
		gameboy->opcodes.opcode_xor(gameboy, gameboy->registers.registers.a, gameboy->bus.ReadByte(gameboy, gameboy->registers.GetHL()));
		pc += 1;
		break;
	case 0xAF:
		if (gameboy->debug) {
			printf("[Debug] Instruction XOR called with arguments: A(Register), A(Register)\n");
		}
		gameboy->opcodes.opcode_xor(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.a);
		pc += 1;
		break;
	case 0xB0:
		if (gameboy->debug) {
			printf("[Debug] Instruction OR called with arguments: A(Register), B(Register)\n");
		}
		gameboy->opcodes.opcode_or(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.b);
		pc += 1;
		break;
	case 0xB1:
		if (gameboy->debug) {
			printf("[Debug] Instruction OR called with arguments: A(Register), C(Register)\n");
		}
		gameboy->opcodes.opcode_or(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.c);
		pc += 1;
		break;
	case 0xB2:
		if (gameboy->debug) {
			printf("[Debug] Instruction OR called with arguments: A(Register), D(Register)\n");
		}
		gameboy->opcodes.opcode_or(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.d);
		pc += 1;
		break;
	case 0xB3:
		if (gameboy->debug) {
			printf("[Debug] Instruction OR called with arguments: A(Register), E(Register)\n");
		}
		gameboy->opcodes.opcode_or(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.e);
		pc += 1;
		break;
	case 0xB4:
		if (gameboy->debug) {
			printf("[Debug] Instruction OR called with arguments: A(Register), H(Register)\n");
		}
		gameboy->opcodes.opcode_or(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.h);
		pc += 1;
		break;
	case 0xB5:
		if (gameboy->debug) {
			printf("[Debug] Instruction OR called with arguments: A(Register), L(Register)\n");
		}
		gameboy->opcodes.opcode_or(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.l);
		pc += 1;
		break;
	case 0xB6:
		if (gameboy->debug) {
			printf("[Debug] Instruction OR called with arguments: A(Register), (HL)\n");
		}
		gameboy->opcodes.opcode_or(gameboy, gameboy->registers.registers.a, gameboy->bus.ReadByte(gameboy, gameboy->registers.GetHL()));
		pc += 1;
		break;
	case 0xB7:
		if (gameboy->debug) {
			printf("[Debug] Instruction OR called with arguments: A(Register), A(Register)\n");
		}
		gameboy->opcodes.opcode_or(gameboy, gameboy->registers.registers.a, gameboy->registers.registers.a);
		pc += 1;
		break;
	case 0xC3: {
		gameboy->opcodes.opcode_jp_mem(gameboy);
		if (gameboy->debug) {
			printf("[Debug] Instruction JP called, pc is now at location: 0x%X\n", pc);
		}
		break;
	}
	case 0xC8: {
		if (gameboy->debug) {
			printf("[Debug] Instruction RST called on 0x00, pc is now at location: 0x%X\n", pc);
		}
		pc += 1;
		gameboy->opcodes.opcode_rst(gameboy, 0x00);
		break;
	}
	case 0xCF: {
		if (gameboy->debug) {
			printf("[Debug] Instruction RST called on 0x08, pc is now at location: 0x%X\n", pc);
		}
		pc += 1;
		gameboy->opcodes.opcode_rst(gameboy, 0x08);
		break;
	}
	case 0xD8: {
		if (gameboy->debug) {
			printf("[Debug] Instruction RST called on 0x10, pc is now at location: 0x%X\n", pc);
		}
		pc += 1;
		gameboy->opcodes.opcode_rst(gameboy, 0x10);
		break;
	}
	case 0xDF: {
		if (gameboy->debug) {
			printf("[Debug] Instruction RST called on 0x18, pc is now at location: 0x%X\n", pc);
		}
		pc += 1;
		gameboy->opcodes.opcode_rst(gameboy, 0x18);
		break;
	}
	case 0xE8: {
		if (gameboy->debug) {
			printf("[Debug] Instruction RST called on 0x20, pc is now at location: 0x%X\n", pc);
		}
		pc += 1;
		gameboy->opcodes.opcode_rst(gameboy, 0x20);
		break;
	}
	case 0xEF: {
		if (gameboy->debug) {
			printf("[Debug] Instruction RST called on 0x28, pc is now at location: 0x%X\n", pc);
		}
		pc += 1;
		gameboy->opcodes.opcode_rst(gameboy, 0x28);
		break;
	}
	case 0xF8: {
		if (gameboy->debug) {
			printf("[Debug] Instruction RST called on 0x30, pc is now at location: 0x%X\n", pc);
		}
		pc += 1;
		gameboy->opcodes.opcode_rst(gameboy, 0x30);
		break;
	}
	case 0xFF: {
		if (gameboy->debug) {
			printf("[Debug] Instruction RST called on 0x38, pc is now at location: 0x%X\n", pc);
		}
		pc += 1;
		gameboy->opcodes.opcode_rst(gameboy, 0x38);
		break;
	}
	default:
		if (gameboy->debug) {
			printf("[Debug] Unimplemented instruction 0x%X\n", instruction);
			system("pause");
		}
		pc += 1;
		break;
	}
	
	printf("\n");
	return Constants::CYCLES[instruction];
}

int CPU::CPU::ExecuteCBInstruction(GBEmu::Gameboy* gameboy, uint8_t instruction) {
	switch (instruction) {
	default:
		if (gameboy->debug) {
			printf("[Debug] Unimplemented CB instruction 0x%X\n", instruction);
			system("pause");
		}
		gameboy->cpu.pc += 2;
		break;
	}
	return Constants::CYCLES[instruction];
}

void CPU::CPU::Push(GBEmu::Gameboy* gameboy, uint16_t value) {
	sp--;
	gameboy->bus.WriteByte(gameboy, sp, (uint8_t)((value & 0xFF00) >> 8));
	sp--;
	gameboy->bus.WriteByte(gameboy, sp, (uint8_t)(value & 0x00FF));
}

uint16_t CPU::CPU::Pop(GBEmu::Gameboy* gameboy) {
	uint16_t value = gameboy->bus.ReadByte(gameboy, sp);
	sp++;
	uint16_t value2 = gameboy->bus.ReadByte(gameboy, sp);
	sp++;
	return (value2 << 8) | value;
}

void CPU::CPU::UpdateTimers(GBEmu::Gameboy* gameboy, int cycles) {
	UpdateDividerRegister(gameboy, cycles);

	if (IsClockEnabled(gameboy)) {
		timer_counter -= cycles;

		if (timer_counter <= 0) {
			SetClockFreq(gameboy);
			if (gameboy->bus.ReadByte(gameboy, Constants::TIMER_ADDRESS) == 255) {
				gameboy->bus.WriteByte(gameboy, Constants::TIMER_ADDRESS, gameboy->bus.ReadByte(gameboy, Constants::TIMER_MODULATOR_ADDRESS));
				RequestInterrupt(gameboy, Interrupt::TIMER);
			}
			else {
				gameboy->bus.WriteByte(gameboy, Constants::TIMER_ADDRESS, (gameboy->bus.ReadByte(gameboy, Constants::TIMER_ADDRESS) + 1));
			}
		}
	}
}

void CPU::CPU::UpdateDividerRegister(GBEmu::Gameboy* gameboy, int cycles) {
	divider_counter += cycles;
	if (divider_counter >= 255) {
		divider_counter = 0;
		gameboy->bus.memory.memory_array[Constants::DIVIDER_REGISTER_ADDRESS]++;
	}
}

bool CPU::CPU::IsClockEnabled(GBEmu::Gameboy* gameboy) {
	return (gameboy->bus.ReadByte(gameboy, Constants::TIMER_CONTROLLER_ADDRESS) & 0b00000100) != 0;
}

uint8_t CPU::CPU::GetClockFreq(GBEmu::Gameboy* gameboy) {
	return (gameboy->bus.ReadByte(gameboy, Constants::TIMER_CONTROLLER_ADDRESS) & 0b00000011);
}

void CPU::CPU::SetClockFreq(GBEmu::Gameboy* gameboy) {
	uint8_t frequency = GetClockFreq(gameboy);
	switch (frequency) {
		case 0: timer_counter = 1024; break; // freq 4096
		case 1: timer_counter = 16; break; // freq 262144
		case 2: timer_counter = 64; break; // freq 65536
		case 3: timer_counter = 256; break; // freq 16382
	}
}

void CPU::CPU::RequestInterrupt(GBEmu::Gameboy* gameboy, Interrupt interrupt) {
	uint8_t request_byte = gameboy->bus.ReadByte(gameboy, Constants::INTERRUPT_REQUEST_REGISTER_ADDRESS);
	switch (interrupt) {
	case Interrupt::VBLANK:
		request_byte |= 0b00000001;
		break;
	case Interrupt::LCD:
		request_byte |= 0b00000010;
		break;
	case Interrupt::TIMER:
		request_byte |= 0b00000100;
		break;
	// Bit 3 is serial, we won't worry about that for now
	case Interrupt::JOYPAD:
		request_byte |= 0b00010000;
		break;
	}
	gameboy->bus.WriteByte(gameboy, Constants::INTERRUPT_REQUEST_REGISTER_ADDRESS, request_byte);
}

void CPU::CPU::HandleInterrupts(GBEmu::Gameboy* gameboy) {
	if (master_interrupts_enabled) {
		uint8_t request_byte = gameboy->bus.ReadByte(gameboy, Constants::INTERRUPT_REQUEST_REGISTER_ADDRESS);
		uint8_t enabled_byte = gameboy->bus.ReadByte(gameboy, Constants::INTERRUPT_ENABLED_REGISTER_ADDRESS);
		if (request_byte > 0) {
			if ((request_byte & 0b00000001) > 0) {
				if ((enabled_byte & 0b00000001) > 0) {
					gameboy->bus.WriteByte(gameboy, Constants::INTERRUPT_REQUEST_REGISTER_ADDRESS, (request_byte & 0b11111110));
					ServiceInterrupt(gameboy, Interrupt::VBLANK);
				}
			}
			if ((request_byte & 0b00000010) > 0) {
				if ((enabled_byte & 0b00000010) > 0) {
					gameboy->bus.WriteByte(gameboy, Constants::INTERRUPT_REQUEST_REGISTER_ADDRESS, (request_byte & 0b11111101));
					ServiceInterrupt(gameboy, Interrupt::LCD);
				}
			}
			if ((request_byte & 0b00000100) > 0) {
				if ((enabled_byte & 0b00000100) > 0) {
					gameboy->bus.WriteByte(gameboy, Constants::INTERRUPT_REQUEST_REGISTER_ADDRESS, (request_byte & 0b11111011));
					ServiceInterrupt(gameboy, Interrupt::TIMER);
				}
			}
			// Again, we aren't worrying about bit 3 for now
			if ((request_byte & 0b00010000) > 0) {
				if ((enabled_byte & 0b00010000) > 0) {
					gameboy->bus.WriteByte(gameboy, Constants::INTERRUPT_REQUEST_REGISTER_ADDRESS, (request_byte & 0b11101111));
					ServiceInterrupt(gameboy, Interrupt::JOYPAD);
				}
			}
		}
	}
}

void CPU::CPU::ServiceInterrupt(GBEmu::Gameboy* gameboy, Interrupt interrupt) {
	master_interrupts_enabled = false;

	Push(gameboy, pc);

	switch (interrupt) {
	case Interrupt::VBLANK:
		pc = 0x0040;
		break;
	case Interrupt::LCD:
		pc = 0x0048;
		break;
	case Interrupt::TIMER:
		pc = 0x0050;
		break;
	case Interrupt::JOYPAD:
		pc = 0x0060;
		break;
	}
	// Unsure if we need to manually reset pc
}