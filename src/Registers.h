#pragma once

namespace GBEmu {
	namespace CPU {
		class Registers {
		public:
			// Emulates the 8 registers of the gameboy's CPU
			struct registers_struct {
				uint8_t a = 0;
				uint8_t b = 0;
				uint8_t c = 0;
				uint8_t d = 0;
				uint8_t e = 0;
				uint8_t f = 0;
				uint8_t h = 0;
				uint8_t l = 0;
			} registers;
			// Emulates the 4 flag registers of the gameboy's CPU
			struct flags_registers_struct {
				bool zero;
				bool subtract;
				bool halfcarry;
				bool carry;
			} flags_registers;

			// Functions
			void SetAF(uint16_t af);
			uint16_t GetAF();
			void SetBC(uint16_t bc);
			uint16_t GetBC();
			void SetDE(uint16_t de);
			uint16_t GetDE();
			void SetHL(uint16_t hl);
			uint16_t GetHL();
			void Registers::SetByteFromFlags();
		};
	}
}