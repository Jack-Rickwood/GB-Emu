#include <iostream>
#include <stdio.h>
#include <assert.h>
#include "Gameboy.h"
#include "GPU.h"
#include "Constants.h"
#include "SDL.h"

using namespace std;
using namespace GBEmu;

void GPU::Initialize() {
	for (int i = 0; i < (144 * 160); i++) {
		screen_data[i] = { 0, 0, 0 };
	}
}

SDL_Color GPU::GetColour(GBEmu::Gameboy* gameboy, bool colour_bit_1, bool colour_bit_2, uint16_t pallete_address) {
	SDL_Color res = Constants::WHITE;
	uint8_t pallete = gameboy->bus.ReadByte(gameboy, pallete_address);
	int hi = 0;
	int lo = 0;
	
	if (colour_bit_1 == false && colour_bit_2 == false) { // 0b00
		hi = 1;
		lo = 0;
	}
	else if (colour_bit_1 == true && colour_bit_2 == false) { // 0b01
		hi = 3;
		lo = 2;
	}
	else if (colour_bit_1 == false && colour_bit_2 == true) { // 0b10
		hi = 5;
		lo = 4;
	}
	else if (colour_bit_1 == true && colour_bit_2 == true) { // 0b11
		hi = 7;
		lo = 6;
	}
	else {
		printf("[ERROR] GetColour");
	}

	int colour = 0;
	colour = (int)gameboy->bus.BitTest(pallete, hi) << 1;
	colour |= (int)gameboy->bus.BitTest(pallete, lo);

	switch (colour) {
	case 0:
		res = Constants::WHITE;
		break;
	case 1:
		res = Constants::LIGHT_GRAY;
		break;
	case 2:
		res = Constants::DARK_GRAY;
		break;
	case 3:
		res = Constants::BLACK;
		break;
	}

	return res;
}

void GPU::UpdateGraphics(GBEmu::Gameboy* gameboy, int cycles) {
	SetLCD(gameboy);

	if (IsLCDEnabled(gameboy)) {
		scanline_counter -= cycles;
	}
	else {
		return;
	}

	if (scanline_counter <= 0) {
		gameboy->bus.memory.memory_array[Constants::LCD_Y_COORD_ADDRESS]++;
		uint8_t current_line = gameboy->bus.ReadByte(gameboy, Constants::LCD_Y_COORD_ADDRESS);

		scanline_counter = Constants::SCANLINE_CYCLE_COUNT;

		if (current_line == 144) {
			gameboy->cpu.RequestInterrupt(gameboy, GBEmu::CPU::CPU::Interrupt::VBLANK);
		}
		else if (current_line > 153) {
			gameboy->bus.memory.memory_array[Constants::LCD_Y_COORD_ADDRESS] = 0;
		}
		else if (current_line < 144) {
			DrawScanLine(gameboy);
		}
	}
}

void GPU::DrawScanLine(GBEmu::Gameboy* gameboy) {
	graphics_info info = GetGraphicsInfo(gameboy);
	if (info.background_display_enabled) {
		RenderBackgroundTiles(gameboy, info);
	}
	if (info.sprite_display_enabled) {
		RenderSprites(gameboy, info);
	}
}

void GPU::RenderBackgroundTiles(GBEmu::Gameboy* gameboy, GPU::graphics_info info) { // Issue here
	uint16_t tile_data_address = 0;
	uint16_t background_memory = 0;
	uint8_t y_position = 0;

	if (!info.window_display_enabled) {
		y_position = gameboy->bus.ReadByte(gameboy, Constants::LCD_Y_COORD_ADDRESS) + info.scroll_y;
		background_memory = info.background_tilemap_start_address;
	}
	else {
		y_position = gameboy->bus.ReadByte(gameboy, Constants::LCD_Y_COORD_ADDRESS) - info.window_y;
		background_memory = info.window_tilemap_start_address;
	}

	// In the 32 * 32 background, what row is the scanline on?
	uint16_t tile_row = (((uint8_t)(y_position / 8)) * 32);

	for (int pixel = 0; pixel < 160; pixel++) {
		uint8_t x_position = pixel + info.scroll_x;
		// Handle wrapping
		if (info.window_display_enabled) {
			if (pixel >= info.window_x) {
				x_position = pixel - info.window_x;
			}
		}
		// In the 32 * 32 background, what column is the scanline on?
		uint16_t tile_column = x_position / 8;
		int16_t tile_number;
		uint16_t tile_data_pointer_address = background_memory + tile_row + tile_column;

		tile_data_address = info.background_window_tiledata_start_address;
		if (info.tile_identifier_signed) {
			tile_number = (int8_t)(gameboy->bus.ReadByte(gameboy, tile_data_pointer_address));
			tile_data_address += (tile_number * 16);
		}
		else {
			tile_number = (uint8_t)(gameboy->bus.ReadByte(gameboy, tile_data_pointer_address));
			tile_data_address += ((tile_number + 128) * 16);
		}
		
		uint8_t within_tile_row = y_position % 8;
		within_tile_row *= 2; // Each row takes up 2 bytes of memory
		uint8_t row_data_1 = gameboy->bus.ReadByte(gameboy, tile_data_address + within_tile_row);
		uint8_t row_data_2 = gameboy->bus.ReadByte(gameboy, tile_data_address + within_tile_row + 1);

		int colour_bit = x_position % 8;
		colour_bit -= 7;
		colour_bit *= -1;

		bool colour_bit_1 = gameboy->bus.BitTest(row_data_1, colour_bit);
		bool colour_bit_2 = gameboy->bus.BitTest(row_data_2, colour_bit);

		SDL_Color colour = GetColour(gameboy, colour_bit_1, colour_bit_2, 0xFF47);
		int final_y = gameboy->bus.ReadByte(gameboy, Constants::LCD_Y_COORD_ADDRESS);

		if ((final_y < 0) || (final_y > 143) || (pixel < 0) || (pixel > 159)) {
			continue;
		}

		screen_data[pixel * final_y] = colour;
	}

	for (int i = 0; i < 144 * 160; i++) {
		SDL_Color data = screen_data[i];
		if ((data.r != 0 || data.g != 0 || data.b != 0) && (data.r != 255 || data.g != 255 || data.b != 255)) {
			system("pause");
		}
	}
}

void GPU::RenderSprites(GBEmu::Gameboy* gameboy, GPU::graphics_info info) {
	for (int sprite = 0; sprite < 40; sprite++) {
		uint8_t sprite_index = sprite * 4;
		uint8_t sprite_y_position = gameboy->bus.ReadByte(gameboy, 0xFE00 + sprite_index) - 16;
		uint8_t sprite_x_position = gameboy->bus.ReadByte(gameboy, 0xFE00 + sprite_index + 1) - 8;
		uint8_t tile_location = gameboy->bus.ReadByte(gameboy, 0xFE00 + sprite_index + 2);
		uint8_t attributes = gameboy->bus.ReadByte(gameboy, 0xFE00 + sprite_index + 3);
		bool y_flip = gameboy->bus.BitTest(attributes, 6);
		bool x_flip = gameboy->bus.BitTest(attributes, 5);

		int scanline = gameboy->bus.ReadByte(gameboy, Constants::LCD_Y_COORD_ADDRESS);

		if ((scanline >= sprite_y_position) && (scanline < (sprite_y_position + info.sprite_size[1]))) {
			int line = scanline - sprite_y_position;

			if (y_flip) {
				line -= info.sprite_size[1];
				line *= -1;
			}

			line *= 2;
			uint16_t tile_data_address = (0x8000 + (tile_location * 16)) + line;
			uint8_t sprite_data_1 = gameboy->bus.ReadByte(gameboy, tile_data_address);
			uint8_t sprite_data_2 = gameboy->bus.ReadByte(gameboy, tile_data_address + 1);

			for (int tile_pixel = 7; tile_pixel >= 0; tile_pixel--) {
				int colour_bit = tile_pixel;

				if (x_flip) {
					colour_bit -= 7;
					colour_bit *= -1;
				}

				bool colour_bit_1 = gameboy->bus.BitTest(sprite_data_1, colour_bit);
				bool colour_bit_2 = gameboy->bus.BitTest(sprite_data_2, colour_bit);

				uint16_t colour_address = gameboy->bus.BitTest(attributes, 4) ? 0xFF49 : 0xFF48;
				SDL_Color colour = GetColour(gameboy, colour_bit_1, colour_bit_2, colour_address);

				if (colour.r == Constants::WHITE.r && 
					colour.g == Constants::WHITE.g && // Should probably change this at some point
					colour.b == Constants::WHITE.b) {
					continue;
				}

				int x_pixel = 0 - tile_pixel;
				x_pixel += 7;
				int pixel = sprite_x_position + x_pixel;

				if ((scanline < 0) || (scanline > 143) || (pixel < 0) || (pixel > 159)) {
					continue;
				}

				screen_data[pixel * scanline] = colour;
			}
		}
	}
	for (int i = 0; i < 144 * 160; i++) {
		SDL_Color data = screen_data[i];
		if ((data.r != 0 || data.g != 0 || data.b != 0) && (data.r != 255 || data.g != 255 || data.b != 255)) {
			system("pause");
		}
	}
}

void GPU::Render() {
	assert(GraphicsRenderer && Texture);
	SDL_UpdateTexture(this->Texture, NULL, screen_data, 144);
	SDL_SetRenderDrawColor(GraphicsRenderer, 255, 255, 255, 255);
	SDL_RenderClear(GraphicsRenderer);
	SDL_RenderCopy(GraphicsRenderer, this->Texture, NULL, NULL);
	SDL_RenderPresent(GraphicsRenderer);
}

void GPU::SetLCD(GBEmu::Gameboy* gameboy) {
	uint8_t lcd_status = gameboy->bus.ReadByte(gameboy, Constants::LCD_STATUS_REGISTER_ADDRESS);
	if (!IsLCDEnabled(gameboy)) {
		scanline_counter = Constants::SCANLINE_CYCLE_COUNT;
		gameboy->bus.memory.memory_array[Constants::LCD_Y_COORD_ADDRESS] = 0;
		lcd_status &= 0b11111100;
		lcd_status = gameboy->bus.BitSet(lcd_status, 0);
		gameboy->bus.WriteByte(gameboy, Constants::LCD_STATUS_REGISTER_ADDRESS, lcd_status);
		return;
	}
	
	uint8_t current_line = gameboy->bus.ReadByte(gameboy, Constants::LCD_Y_COORD_ADDRESS);
	uint8_t current_mode = lcd_status & 0b00000011;
	uint8_t mode = 0;
	uint8_t request_interrupt = false;

	// V-Blank (Mode 1) (01)
	if (current_line >= 144) {
		mode = 1;
		lcd_status &= 0b11111100;
		lcd_status = gameboy->bus.BitSet(lcd_status, 0);
		request_interrupt = gameboy->bus.BitTest(lcd_status, 4);
	}
	else {
		int mode_2_bounds = 456 - 80;
		int mode_3_bounds = mode_2_bounds - 172;

		// Searching Sprite Atts (Mode 2) (10)
		if (scanline_counter >= mode_2_bounds) {
			mode = 2;
			lcd_status &= 0b11111100;
			lcd_status = gameboy->bus.BitSet(lcd_status, 1);
			request_interrupt = gameboy->bus.BitTest(lcd_status, 5);
		}
		// Transfering Data to LCD Driver (Mode 3) (11)
		else if (scanline_counter >= mode_3_bounds) {
			mode = 3;
			lcd_status &= 0b11111100;
			lcd_status = gameboy->bus.BitSet(lcd_status, 0);
			lcd_status = gameboy->bus.BitSet(lcd_status, 1);
		}
		// H-Blank (Mode 0) (00)
		else {
			mode = 0;
			lcd_status &= 0b11111100;
			request_interrupt = gameboy->bus.BitTest(lcd_status, 3);
		}
	}

	if (request_interrupt && (mode != current_mode)) {
		gameboy->cpu.RequestInterrupt(gameboy, GBEmu::CPU::CPU::Interrupt::LCD);
	}

	if (current_line == gameboy->bus.ReadByte(gameboy, 0xFF45)) {
		lcd_status = gameboy->bus.BitSet(lcd_status, 2);
		if (gameboy->bus.BitTest(lcd_status, 6)) {
			gameboy->cpu.RequestInterrupt(gameboy, GBEmu::CPU::CPU::Interrupt::LCD);
		}
	}
	else {
		lcd_status = gameboy->bus.BitReset(lcd_status, 2);
	}

	gameboy->bus.WriteByte(gameboy, Constants::LCD_STATUS_REGISTER_ADDRESS, lcd_status);
}

bool GPU::IsLCDEnabled(GBEmu::Gameboy* gameboy) {
	return (gameboy->bus.BitTest(gameboy->bus.ReadByte(gameboy, Constants::LCD_CONTROL_REGISTER_ADDRESS), 7));
}

GPU::graphics_info GPU::GetGraphicsInfo(GBEmu::Gameboy* gameboy) {
	GPU::graphics_info info;
	uint8_t lcd_control_register = gameboy->bus.ReadByte(gameboy, Constants::LCD_CONTROL_REGISTER_ADDRESS);

	info.scroll_x = gameboy->bus.ReadByte(gameboy, 0xFF43);
	info.scroll_y = gameboy->bus.ReadByte(gameboy, 0xFF42);
	info.window_x = gameboy->bus.ReadByte(gameboy, 0xFF4B) - 7;
	info.window_y = gameboy->bus.ReadByte(gameboy, 0xFF4A);

	/* Bit 7 */
	info.lcd_display_enabled = gameboy->bus.BitTest(lcd_control_register, 7);

	/* Bit 6 */
	if (gameboy->bus.BitTest(lcd_control_register, 6)) {
		info.window_tilemap_start_address = 0x9C00;
		info.window_tilemap_end_address = 0x9FFF;
	}
	else {
		info.window_tilemap_start_address = 0x9800;
		info.window_tilemap_end_address = 0x9BFF;
	}

	/* Bit 5 */
	if (gameboy->bus.BitTest(lcd_control_register, 5)) {
		if (info.window_y <= gameboy->bus.ReadByte(gameboy, Constants::LCD_Y_COORD_ADDRESS)) {
			info.window_display_enabled = true;
		}
		else {
			info.window_display_enabled = false;
		}
	}
	else {
		info.window_display_enabled = false;
	}

	/* Bit 4 */
	if (gameboy->bus.BitTest(lcd_control_register, 4)) {
		info.background_window_tiledata_start_address = 0x8000;
		info.background_window_tiledata_end_address = 0x8FFF;
		info.tile_identifier_signed = false;
	}
	else {
		info.background_window_tiledata_start_address = 0x8800;
		info.background_window_tiledata_end_address = 0x97FF;
		info.tile_identifier_signed = true;
	}

	/* Bit 3 */
	if (gameboy->bus.BitTest(lcd_control_register, 3)) {
		info.background_tilemap_start_address = 0x9C00;
		info.background_tilemap_end_address = 0x9FFF;
	}
	else {
		info.background_tilemap_start_address = 0x9800;
		info.background_tilemap_end_address = 0x9BFF;
	}

	/* Bit 2 */
	if (gameboy->bus.BitTest(lcd_control_register, 2)) {
		info.sprite_size[0] = 8;
		info.sprite_size[1] = 16;
	}
	else {
		info.sprite_size[0] = 8;
		info.sprite_size[1] = 8;
	}

	/* Bit 1 */
	info.sprite_display_enabled = gameboy->bus.BitTest(lcd_control_register, 1);

	/* Bit 0 */
	info.background_display_enabled = gameboy->bus.BitTest(lcd_control_register, 0);

	return info;
}