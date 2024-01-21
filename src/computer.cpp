#include "computer.h"

bool timerRunning = false;
bool execute = false;
uint32_t timeInterval = 10000;
int timerID = -1;


void decimalpoint2tostr(char* output, int input) {
    char* value = (char *) malloc(5);
    snprintf(value, 4, "%03d", input);
    value[3] = value[2];
    value[2] = value[1];
    value[1] = '.';
    value[4] = '\0';
    strcpy(output, value);
    free(value);
}


void runWindow() {
    timer_pause(timerID);

    char* decimal2 = (char *)malloc(5);

    if (timerRunning)
    {
        dtext(126, 67, C_GREEN, "Running (Paused)");
    }
    else {
        dtext(126, 67, C_RED, "Stopped");
    }

    uint8_t slider = (timeInterval == 10000) ? 0 : (timeInterval / 50000);

    decimalpoint2tostr(decimal2, timeInterval / 10000);

    drect_border(146, 77, 249, 81, C_WHITE, 2, C_BLACK);
    drect(147 + slider, 78, 149 + slider, 80, C_WHITE);
    dprint_opt(126, 85, C_WHITE, C_BLACK, DTEXT_LEFT, DTEXT_TOP, "%s s", decimal2);

    dtext(126, 135, C_WHITE, "AC/ON: Reset");
    dtext(126, 143, C_WHITE, "EXIT: Cancel EXE: Submit");
    dupdate();

    key_event_t key;
    do {
        key = getkey();
        switch (key.key)
        {
        case KEY_LEFT:
            if (slider != 0)
            {
                slider--;
            }
            break;
        case KEY_RIGHT:
            if (slider != 100)
            {
                slider++;
            }
            break;
        case KEY_EXIT:
            timer_start(timerID);
            free(decimal2);
            return;
        case KEY_ACON:
            timer_stop(timerID);
            execute = false;
            timerRunning = false;
            free(decimal2);
            return;
        default:
            break;
        }

        timeInterval = (slider != 0) ? slider * 50000 : 10000;

        decimalpoint2tostr(decimal2, timeInterval / 10000);

        drect_border(146, 77, 249, 81, C_WHITE, 2, C_BLACK);
        drect(147 + slider, 78, 149 + slider, 80, C_WHITE);
        dprint_opt(126, 85, C_WHITE, C_BLACK, DTEXT_LEFT, DTEXT_TOP, "%s s", decimal2);

        dupdate();
    }
    while (key.key != KEY_EXE);

    if (slider == 0) {
        timerID = timer_configure(TIMER_ANY, timeInterval, GINT_CALL(runExecute));
    }
    else {
        timerID = timer_configure(TIMER_ANY, timeInterval, GINT_CALL(runExecute));
    }
    timerRunning = true;
    timer_start(timerID);
    
    free(decimal2);
    runExecute();
}


int runExecute() {
    execute = true;
    return TIMER_CONTINUE;
}


void rangeWindow(uint16_t *rangeAddress, uint8_t *memory) {
    char* rangeBuffer = (char *)malloc(5);
    memset(rangeBuffer, '0', 5);
    rangeBuffer[4] = '\0';

    dtext(126, 135, C_WHITE, "AC/ON: Reset ");
    dtext(126, 143, C_WHITE, "EXIT: Cancel EXE: Submit");
    dupdate();
    
    dtext(126, 67, C_WHITE, "Start Address:");

    int *kopt_timeout = (int*) malloc(sizeof(kopt_timeout));
    *kopt_timeout = 1;
    int cursor = 0;
    key_event_t key;
    do
    {
        key = getkey_opt(GETKEY_DEFAULT, kopt_timeout);
        if (key.type == KEYEV_DOWN || key.type == KEYEV_HOLD)
        {
            switch (key.key)
            {
            case KEY_EXIT:
                free(kopt_timeout);
                free(rangeBuffer);
                return;
            case KEY_RIGHT:
                cursor++;
                if (cursor > 3) cursor = 0;
                break;
            case KEY_LEFT:
                if (cursor == 0) cursor = 3;
                else cursor--;
                break;
            case KEY_ACON:
                *rangeAddress = (memory[0xFFFD] << 8) + memory[0xFFFC];
                free(kopt_timeout);
                free(rangeBuffer);
                return;
            default:
                break;
            }

            if (keycode_custom(KEYCODE_A_F | KEYCODE_DEC, key) != -1)
            {
                rangeBuffer[cursor] = keycode_custom(KEYCODE_A_F | KEYCODE_DEC, key);
                cursor++;
                if (cursor > 3) cursor = 0;
            }
        }
        
        dline(126, 83, 148, 83, C_BLACK);
        dline(126 + 6 * cursor, 83, 130 + 6 * cursor, 83, C_WHITE);
        dprint_opt(126, 75, C_WHITE, C_BLACK, DTEXT_LEFT, DTEXT_TOP, "%s", rangeBuffer);
        dupdate();
    } while (key.key != KEY_EXE);

    *rangeAddress = strwordhexToInt(rangeBuffer);

    free(kopt_timeout);
    free(rangeBuffer);
}


void editMode(uint16_t *rangeAddress, uint8_t *memory, mos6502 computer) {
    timer_pause(timerID);

    uint8_t *memoryBuffer = (uint8_t *) malloc(0xFFFF);

    memcpy(memoryBuffer, memory, 0xFFFF);

    drawFunctions("Save As", "Load", "Range", "", "Assembly", "Save");

    uint8_t cursor = 0;

    image_t *tmpWindow;

    key_event_t key;
    while (true) {
        key = pollevent();
        if (key.type == KEYEV_DOWN || key.type == KEYEV_HOLD)
        {
            switch (key.key)
            {
            case KEY_F1:
                free(memoryBuffer);
                timer_start(timerID);
                return;
            case KEY_F2:
                free(memoryBuffer);
                timer_start(timerID);
                return;
            case KEY_F3:
                tmpWindow = drawWindow("Range");
				rangeWindow(rangeAddress, memoryBuffer);
				freeWindow(&tmpWindow);
                break;
            case KEY_EXIT:
                free(memoryBuffer);
                timer_start(timerID);
                return;
            case KEY_F5:
                break;
            case KEY_F6:
                memcpy(memory, memoryBuffer, 0xFFFF);
                
                free(memoryBuffer);
                timer_start(timerID);
                return;
            
            case KEY_RIGHT:
                cursor++;
                break;
            case KEY_LEFT:
                cursor--;
                break;
            case KEY_UP:
                cursor -= 16;
                break;
            case KEY_DOWN:
                cursor += 16;
                break;
            case KEY_EXE:
                memoryBuffer[(*rangeAddress & 0xFFF0) + cursor] = editData(cursor, memoryBuffer[(*rangeAddress & 0xFFF0) + cursor], '\0');
                break;
            default:
                break;
            }

            if (keycode_custom(KEYCODE_A_F | KEYCODE_DEC, key) != -1)
            {
                memoryBuffer[(*rangeAddress & 0xFFF0) + cursor] = editData(cursor, memoryBuffer[(*rangeAddress & 0xFFF0) + cursor], keycode_custom(KEYCODE_A_F | KEYCODE_DEC, key));
            }
        }

        drawMemoryMode(true);
		drawMemory(memoryBuffer, *rangeAddress, (*rangeAddress & 0xFFF0) + cursor);
		drawRegisters(computer);
		dupdate();
    }
}

#define TOPLEFT_VAL_X  37
#define TOPLEFT_VAL_Y  26

#define VAL_OFFSET_X (5 * 2 + 1 + 3)
#define VAL_OFFSET_Y (7 + 4)

uint8_t editData(uint8_t memoryCursor, uint8_t initialData, char initialKey) {
    char* dataBuffer = (char *)malloc(3);
    memset(dataBuffer, '\0', 3);
    sprintf(dataBuffer, "%02X", initialData);

    if (initialKey != '\0') {
        dataBuffer[0] = initialKey;
    }

    int cursor = 0;

    int *kopt_timeout = (int*) malloc(sizeof(kopt_timeout));
    *kopt_timeout = 1;
    key_event_t key;
    do
    {
        key = getkey_opt(GETKEY_DEFAULT, kopt_timeout);
        if (key.type == KEYEV_DOWN || key.type == KEYEV_HOLD)
        {
            switch (key.key)
            {
            case KEY_EXIT:
                free(kopt_timeout);
                free(dataBuffer);
                return initialData;
            default:
                break;
            }

            if (keycode_custom(KEYCODE_A_F | KEYCODE_DEC, key) != -1)
            {
                dataBuffer[cursor] = keycode_custom(KEYCODE_A_F | KEYCODE_DEC, key);
                cursor++;
                if (cursor > 1) cursor = 0;
            }
        }

        drect(TOPLEFT_VAL_X + VAL_OFFSET_X * (memoryCursor & 0xF) - 1, 
            TOPLEFT_VAL_Y + VAL_OFFSET_Y * (memoryCursor >> 4) - 1,
            TOPLEFT_VAL_X + VAL_OFFSET_X * (memoryCursor & 0xF) + 2 * 5 + 1,
            TOPLEFT_VAL_Y + VAL_OFFSET_Y * (memoryCursor >> 4) + 7,
            0xFEBA
        );

        dprint_opt(TOPLEFT_VAL_X + VAL_OFFSET_X * (memoryCursor & 0xF),
            TOPLEFT_VAL_Y + VAL_OFFSET_Y * (memoryCursor >> 4), 
            C_BLACK,
            0xFEBA,
            DTEXT_LEFT,
            DTEXT_TOP,
            "%s", 
            dataBuffer
        );

        dupdate();
    } while (key.key != KEY_EXE);

    uint8_t data = strbytehexToInt(dataBuffer);

    free(kopt_timeout);
    free(dataBuffer);

    return data;
}
