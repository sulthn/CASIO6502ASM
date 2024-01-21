#pragma once

#include <gint/display.h>
#include <gint/keyboard.h>

#include <cstring>
#include <cstdlib>


enum {
    KEYCODE_A_F = 0x01,
    KEYCODE_G_L = 0x02,
    KEYCODE_M_O = 0x04,
    KEYCODE_P_T = 0x08,
    KEYCODE_U_Z = 0x10,
    KEYCODE_DEC = 0x20
};

char keycode_custom(int options, key_event_t key);

uint16_t strwordhexToInt(const char* str);

uint8_t strbytehexToInt(const char* str);