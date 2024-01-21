#include "input.h"

// ROW, COL
char keycode_custom(int options, key_event_t key) {
    char value = -1;

    int col = 0;
    int row = 0;
    if (options & KEYCODE_DEC)
    {
        if (keycode_digit(key.key) != -1)
        {
            value = '0' + keycode_digit(key.key);
        }
    }
    if (value == -1 && options & KEYCODE_A_F) {
        row = key.key >> 4;
        col = key.key & 0x0f;
        if (row > 6 || row < 6)
        {
            value = -1;
            return value;
        }
        value = 'A' + (col - 1);
    }
    
    return value;
}

uint16_t strwordhexToInt(const char* str) {
    uint16_t tmp = 0;
    for (int i = 0; i < 4; i++) {
        tmp <<= 4;
        if (str[i] >= 'A' && str[i] <= 'F') 
            tmp += str[i] - 'A' + 10;
        else if (str[i] >= '0' && str[i] <= '9') 
            tmp += str[i] - '0';
    }

    return tmp;
}

uint8_t strbytehexToInt(const char* str) {
    uint8_t tmp = 0;
    for (int i = 0; i < 2; i++) {
        tmp <<= 4;
        if (str[i] >= 'A' && str[i] <= 'F') 
            tmp += str[i] - 'A' + 10;
        else if (str[i] >= '0' && str[i] <= '9') 
            tmp += str[i] - '0';
    }

    return tmp;
}
