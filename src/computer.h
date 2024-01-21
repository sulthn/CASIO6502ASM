#pragma once

#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/gint.h>
#include <gint/timer.h>

#include "mos6502/mos6502.h"
#include "input.h"
#include "graphics.h"

#include <cstdio>
#include <cstdlib>

extern bool timerRunning;
extern bool execute;
extern int timerID;

void runWindow();

int runExecute();

void rangeWindow(uint16_t *rangeAddress, uint8_t *memory);

void editMode(uint16_t *rangeAddress, uint8_t *memory, mos6502 computer);

uint8_t editData(uint8_t memoryCursor, uint8_t initialData, char initialKey);
