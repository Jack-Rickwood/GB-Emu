#include "SDL.h"
#ifndef EMU_CONSTANTS_H
#define EMU_CONSTANTS_H
#pragma once

namespace GBEmu {
	namespace Constants {
		// Cycles
        const uint8_t CYCLES[256] = {
            1, 3, 2, 2, 1, 1, 2, 1, 5, 2, 2, 2, 1, 1, 2, 1,
            1, 3, 2, 2, 1, 1, 2, 1, 3, 2, 2, 2, 1, 1, 2, 1,
            2, 3, 2, 2, 1, 1, 2, 1, 2, 2, 2, 2, 1, 1, 2, 1,
            2, 3, 2, 2, 3, 3, 3, 1, 2, 2, 2, 2, 1, 1, 2, 1,
            1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
            1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
            1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
            2, 2, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1,
            1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
            1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
            1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
            1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
            2, 3, 3, 4, 3, 4, 2, 4, 2, 4, 3, 0, 3, 6, 2, 4,
            2, 3, 3, 0, 3, 4, 2, 4, 2, 4, 3, 0, 3, 0, 2, 4,
            3, 3, 2, 0, 0, 4, 2, 4, 4, 1, 4, 0, 0, 0, 2, 4,
            3, 3, 2, 1, 0, 4, 2, 4, 3, 2, 4, 1, 0, 0, 2, 4
        };

        const uint8_t CB_CYCLES[256] = {
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
            2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
            2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
            2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2,
            2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2
        };

		// Addresses
		const uint16_t ROM_START_ADDRESS = 0x0100;
		const uint16_t RAM_START_ADDRESS = 0x0000;
		const uint16_t RAM_END_ADDRESS = 0xFFFF;
		const uint16_t RAM_SIZE = RAM_END_ADDRESS - RAM_START_ADDRESS;
		const uint16_t VRAM_START_ADDRESS = 0x8000;
		const uint16_t VRAM_END_ADDRESS = 0x9FFF;
		const uint16_t VRAM_SIZE = VRAM_END_ADDRESS - VRAM_START_ADDRESS;
        const uint16_t DIVIDER_REGISTER_ADDRESS = 0xFF04;
		const uint16_t TIMER_ADDRESS = 0xFF05;
		const uint16_t TIMER_MODULATOR_ADDRESS = 0xFF06;
		const uint16_t TIMER_CONTROLLER_ADDRESS = 0xFF07;
		const uint16_t LCD_CONTROL_REGISTER_ADDRESS = 0xFF40;
        const uint16_t LCD_STATUS_REGISTER_ADDRESS = 0xFF41;
		const uint16_t LCD_Y_COORD_ADDRESS = 0xFF44;
        const uint16_t INTERRUPT_REQUEST_REGISTER_ADDRESS = 0xFF0F;
        const uint16_t INTERRUPT_ENABLED_REGISTER_ADDRESS = 0xFFFF;

		// Colors
		const SDL_Color WHITE = { 255, 255, 255 };
		const SDL_Color LIGHT_GRAY = { 170, 170, 170 };
		const SDL_Color DARK_GRAY = { 85, 85, 85 };
		const SDL_Color BLACK = { 0, 0, 0 };

		// Other
		const int MAX_CYCLES = 69905;
		const int SCANLINE_CYCLE_COUNT = 456;
	};
}

#endif