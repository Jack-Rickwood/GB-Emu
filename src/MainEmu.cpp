#include <iostream>
#include <stdio.h>
#include "Gameboy.h"

using namespace std;

int main(int argc, char** argv) {
	GBEmu::Gameboy gameboy;
	bool debug_flag = false;
	// Load Rom
	if (argc == 1) {
		cerr << "[ERROR] " << "ROM could not be found. Did you provide one?" << endl;
	}
	else if (argc == 2) {
		cout << "[INFO] " << "Loading the provided ROM..." << endl;
		gameboy.LoadRom(argv[argc - 1], debug_flag);
	}
	else if (argc == 3) {
		string debug_arg = argv[argc - 1];
		if (debug_arg == "--debug") {
			debug_flag = true;
			cout << "[INFO] " << "Loading the provided ROM in debug mode..." << endl;
			gameboy.LoadRom(argv[argc - 2], debug_flag);
		}
		else {
			cerr << "[ERROR] " << "Usage is <path-to-rom> [--debug]" << endl;
		}
	}
	else {
		cerr << "[ERROR] " << "Usage is <path-to-rom> [--debug]" << endl;
	}
	return 0;
}