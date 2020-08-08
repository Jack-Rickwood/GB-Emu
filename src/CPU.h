#pragma once

namespace GBEmu {
	class Gameboy;
	namespace CPU {
		// The class that emulates the gameboy's CPU
		class CPU {
		public:
			enum class Interrupt {
				VBLANK,
				LCD,
				TIMER,
				JOYPAD
			};

			// Vars
			uint16_t pc;
			uint16_t sp;
			int timer_counter = 0;
			int divider_counter = 0;
			bool master_interrupts_enabled = false;

			// Functions
			int HandleInstruction(GBEmu::Gameboy* gameboy);
			int ExecuteInstruction(GBEmu::Gameboy* gameboy, uint8_t instruction);
			int ExecuteCBInstruction(GBEmu::Gameboy* gameboy, uint8_t instruction);
			void Push(GBEmu::Gameboy* gameboy, uint16_t value);
			uint16_t Pop(GBEmu::Gameboy* gameboy);
			void UpdateTimers(GBEmu::Gameboy* gameboy, int cycles);
			void UpdateDividerRegister(GBEmu::Gameboy* gameboy, int cycles);
			bool IsClockEnabled(GBEmu::Gameboy* gameboy);
			uint8_t GetClockFreq(GBEmu::Gameboy* gameboy);
			void SetClockFreq(GBEmu::Gameboy* gameboy);
			void RequestInterrupt(GBEmu::Gameboy* gameboy, Interrupt interrupt);
			void HandleInterrupts(GBEmu::Gameboy* gameboy);
			void ServiceInterrupt(GBEmu::Gameboy* gameboy, Interrupt interrupt);
		};
		class Opcodes {
		public:
			// Opcode Functions
			/* INC */
			void opcode_inc_set_flags(GBEmu::Gameboy* gameboy, uint8_t value);
			void opcode_inc_hl_deref(GBEmu::Gameboy* gameboy);

			/* DEC */
			void opcode_dec_set_flags(GBEmu::Gameboy* gameboy, uint8_t value);
			void opcode_dec_hl_deref(GBEmu::Gameboy* gameboy);

			/* CPL */
			void opcode_cpl(GBEmu::Gameboy* gameboy);

			/* LD */
			void opcode_ld_a(GBEmu::Gameboy* gameboy, uint8_t value);
			void opcode_ld_b(GBEmu::Gameboy* gameboy, uint8_t value);
			void opcode_ld_c(GBEmu::Gameboy* gameboy, uint8_t value);
			void opcode_ld_d(GBEmu::Gameboy* gameboy, uint8_t value);
			void opcode_ld_e(GBEmu::Gameboy* gameboy, uint8_t value);
			void opcode_ld_h(GBEmu::Gameboy* gameboy, uint8_t value);
			void opcode_ld_l(GBEmu::Gameboy* gameboy, uint8_t value);
			void opcode_ld_bc(GBEmu::Gameboy* gameboy);
			void opcode_ld_bc_deref(GBEmu::Gameboy* gameboy, uint8_t value);
			void opcode_ld_de(GBEmu::Gameboy* gameboy);
			void opcode_ld_de_deref(GBEmu::Gameboy* gameboy, uint8_t value);
			void opcode_ld_hl(GBEmu::Gameboy* gameboy);
			void opcode_ld_hl_deref(GBEmu::Gameboy* gameboy, uint8_t value);
			void opcode_ld_sp(GBEmu::Gameboy* gameboy);
			void opcode_ld_mem(GBEmu::Gameboy* gameboy, uint16_t value);
			void opcode_rst(GBEmu::Gameboy* gameboy, uint16_t address);

			/* ADD */
			void opcode_add(GBEmu::Gameboy* gameboy, uint8_t value1, uint8_t value2);

			/* ADC */
			void opcode_adc(GBEmu::Gameboy* gameboy, uint8_t value1, uint8_t value2);

			/* SUB */
			void opcode_sub(GBEmu::Gameboy* gameboy, uint8_t value1, uint8_t value2);

			/* SBC */
			void opcode_sbc(GBEmu::Gameboy* gameboy, uint8_t value1, uint8_t value2);

			/* AND */
			void opcode_and(GBEmu::Gameboy* gameboy, uint8_t value1, uint8_t value2);

			/* XOR */
			void opcode_xor(GBEmu::Gameboy* gameboy, uint8_t value1, uint8_t value2);

			/* OR */
			void opcode_or(GBEmu::Gameboy* gameboy, uint8_t value1, uint8_t value2);

			/* JP */
			void opcode_jp(GBEmu::Gameboy* gameboy, uint16_t address);
			void opcode_jp_mem(GBEmu::Gameboy* gameboy);
			void opcode_jp_reg(GBEmu::Gameboy* gameboy);
		};
	}
}