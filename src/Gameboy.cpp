#include <iostream>
#include <stdio.h>
#include "Constants.h"
#include "Gameboy.h"
#include "CPU.h"
#include "GPU.h"
#include "SDL.h"

using namespace std;

void GBEmu::Gameboy::LoadRom(string rompath, bool debug_mode) {
	cout << "[INFO] " << "ROM found at: " + rompath + ". Loading..." << endl;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "[ERROR] " << "SLD_INIT failed!" << endl;
	}
	this->gpu.GraphicsWindow = SDL_CreateWindow("Emulation Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 800, SDL_WINDOW_SHOWN);
	this->gpu.GraphicsRenderer = SDL_CreateRenderer(this->gpu.GraphicsWindow, -1, 0);
	SDL_RenderSetLogicalSize(this->gpu.GraphicsRenderer, 720, 800);
	this->gpu.Texture = SDL_CreateTexture(this->gpu.GraphicsRenderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 144, 160);

	this->debug = debug_mode;
	this->bus.Initialize(this);
	this->gpu.Initialize();
	this->bus.LoadRom(this, rompath);
	while (true) {
		Update(this);
	}
}

void GBEmu::Gameboy::Update(GBEmu::Gameboy* gameboy) {
	int cycles_this_update = 0;

	while (cycles_this_update < Constants::MAX_CYCLES) {
		int cycles = gameboy->cpu.HandleInstruction(gameboy);
		cycles_this_update += cycles;
		gameboy->cpu.UpdateTimers(gameboy, cycles);
		gameboy->gpu.UpdateGraphics(gameboy, cycles);
		gameboy->cpu.HandleInterrupts(gameboy);
	}
	
	gameboy->gpu.Render();
}