#include <SDL.h>
#include "Constants.h"
#pragma once

namespace GBEmu {
	class Gameboy;
	// The class that emulates the gameboy's GPU
	class GPU {
	private:
		int scanline_counter;

		struct graphics_info {
			bool lcd_display_enabled = false; // Bit 7
			uint16_t window_tilemap_start_address = 0; // Bit 6
			uint16_t window_tilemap_end_address = 0; // Bit 6
			bool window_display_enabled = false; // Bit 5
			uint16_t background_window_tiledata_start_address = 0; // Bit 4
			uint16_t background_window_tiledata_end_address = 0; // Bit 4
			bool tile_identifier_signed = false;
			uint16_t background_tilemap_start_address = 0; // Bit 3
			uint16_t background_tilemap_end_address = 0; // Bit 3
			int sprite_size[2] = {0,0}; // Bit 2
			bool sprite_display_enabled = false; // Bit 1
			bool background_display_enabled = false; // Bit 0 (Check when implementing colour)

			uint8_t scroll_x = 0; // 0xFF43
			uint8_t scroll_y = 0; // 0xFF42
			uint8_t window_x = 0; // 0xFF4B
			uint8_t window_y = 0; // 0xFF4A

			const int size_of_tile = 16;
			const int offset = 128;
		};

		SDL_Color screen_data[144 * 160];

	public:
		SDL_Window* GraphicsWindow;
		SDL_Renderer* GraphicsRenderer;
		SDL_Texture* Texture;

		// Functions
		void Initialize();
		SDL_Color GetColour(GBEmu::Gameboy* gameboy, bool colour_bit_1, bool colour_bit_2, uint16_t pallete_address);
		void UpdateGraphics(GBEmu::Gameboy* gameboy, int cycles);
		void DrawScanLine(GBEmu::Gameboy* gameboy);
		void RenderBackgroundTiles(GBEmu::Gameboy* gameboy, graphics_info info);
		void RenderSprites(GBEmu::Gameboy* gameboy, graphics_info info);
		void Render();
		void SetLCD(GBEmu::Gameboy* gameboy);
		bool IsLCDEnabled(GBEmu::Gameboy* gameboy);
		GPU::graphics_info GetGraphicsInfo(GBEmu::Gameboy* gameboy);
	};
}