# GB-Emu
A WIP emulator for the Gameboy, written in C++

## Current Progress
Currently no games display correctly, and there is no input or sound support. A large portion of the CPU is done, and the foundations of the graphics are too, however there are probably quite a few bugs i'll have to iron out before any games display correctly. Images of what currently *is* displaying can be found below. Only a 32kb rom loader is implemented and ROM/RAM bank switching is also currently unimplemented.
### Tetris
![alt-text](https://github.com/Jack-Rickwood/GB-Emu/blob/master/images/Tetris_Broken.png "Tetris")
Tetris is one of the few games that progresses to where it will try to display something, however as the image above shows, it isn't correct just yet.

## Usage
Simply run the exe with the path to your ROM as an argument. Running a ROM with the debug flag will display debug information such as the current opcode, and what data is currently in each of the registers.

## Build Requirements
- Requires SDL2 development library which you can get from [here](https://www.libsdl.org/download-2.0.php).