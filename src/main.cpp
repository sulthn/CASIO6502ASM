#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/gint.h>

#include "mos6502/mos6502.h"
#include "computer.h"
#include "graphics.h"

#include <cstdio>
#include <cstring>

using namespace std;

uint8_t prgm[] = {0xa9, 0x01, 0x6a, 0x4c, 0x02, 0x02};

uint8_t memorySpace[0xFFFF + 1] = { 0x00 };

uint8_t read(uint16_t addr) {
	return memorySpace[addr];
}

void write(uint16_t addr, uint8_t val) {
	memorySpace[addr] = val;
}

mos6502 cpu(read, write);

uint16_t rangeAddress = 0x0000;
uint64_t cycleCount;

image_t* tmpWindow;

bool memoryEditState = false;

int main(void)
{
	// Copy default program to memory.
	memcpy(memorySpace + 0x0200, prgm, sizeof(prgm));

	memorySpace[0xFFFC] = 0x00;
	memorySpace[0xFFFD] = 0x02;

	setupHome();
	setupComputer(cpu, memorySpace);

	rangeAddress = (memorySpace[0xFFFD] << 8) + memorySpace[0xFFFC];

	cpu.SetResetP(0b00000100);
    cpu.Reset();

	// Main loop
	while (true) {
		key_event_t key = pollevent();
		if (key.type == KEYEV_DOWN)
		{
			switch (key.key)
			{
			case KEY_F1: // Run
				tmpWindow = drawWindow("Run");
				runWindow();
				freeWindow(&tmpWindow);
				break;
			case KEY_F2: // Reset
				rangeAddress = (memorySpace[0xFFFD] << 8) + memorySpace[0xFFFC];
				cpu.SetResetP(0b00000100);
    			cpu.Reset();
				break;
			case KEY_F3: // Range
				timer_pause(timerID);
				tmpWindow = drawWindow("Range");
				rangeWindow(&rangeAddress, memorySpace);
				freeWindow(&tmpWindow);
				timer_start(timerID);
				break;
			case KEY_F4: // 
				break;
			case KEY_F5: // Interrupt
				break;
			case KEY_F6: // Edit
				editMode(&rangeAddress, memorySpace, cpu);
				setupFunctions();
				break;

			case KEY_MENU:
				gint_osmenu();
				break;
			case KEY_EXE:
				cpu.Run(1, cycleCount);
				break;
			case KEY_OPTN:
				break;
			
			default:
				break;
			}
		}

		if (execute) {
			cpu.Run(1, cycleCount);
			execute = false;
			timer_start(timerID);
		}
		drawMemoryMode(false);
		drawMemory(memorySpace, rangeAddress, cpu.GetPC());
		drawRegisters(cpu);
		dupdate();
	}
	return 1;
}

