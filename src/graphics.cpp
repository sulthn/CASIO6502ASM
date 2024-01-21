#include "graphics.h"

void setupHome() {
    dclear(C_BLACK);
    dfont(&gint_font5x7); // Set font.

    // Memory window.
    drect_border(2, 2, 264, 203, C_BLACK, 1, C_WHITE);
    dline(2, 13, 263, 13, C_WHITE);
    dline(34, 16, 34, 201, C_WHITE);
    dtext(5, 5, C_WHITE, "Memory (1)");

    // Registers and disassembler.
    drect_border(268, 2, 394, 203, C_BLACK, 1, C_WHITE);
    dline(269, 13, 393, 13, C_WHITE);
    dtext(271, 5, C_WHITE, "Registers (2)");

    dline(269, 90, 393, 90, C_WHITE);
    dline(269, 101, 393, 101, C_WHITE);
    dtext(271, 93, C_WHITE, "Disassembler (3)");

    // Function buttons.
    setupFunctions();
    
    dupdate();
}

void setupComputer(mos6502 mpu, uint8_t* memory) {
    // Reset and set reset interrupt inhibit.
    mpu.SetResetP(0b00000100);
    mpu.Reset();

    // Draw memory.
    drawMemory(memory, 0x0000, mpu.GetPC());

    // Draw registers.
    drawRegisters(mpu);
}

void setupFunctions() {
    drawFunctions("Run", "Reset", "Range", "", "", "Edit");
}

void drawFunctions(const char* F1, const char* F2, const char* F3, const char* F4, const char* F5, const char* F6) {
    drect(0, 206, 395, 223, C_BLACK);
    for (int i = 0; i < 6; i++)
    {
        drect_border(66 * i, 206, 65 + 66 * i, 223, C_BLACK, 1, C_WHITE);
    }
    dtext_opt(32 + 66 * 0, 214, C_WHITE, C_BLACK, DTEXT_MIDDLE, DTEXT_MIDDLE, F1, -1); // F1
    dtext_opt(32 + 66 * 1, 214, C_WHITE, C_BLACK, DTEXT_MIDDLE, DTEXT_MIDDLE, F2, -1); // F2
    dtext_opt(32 + 66 * 2, 214, C_WHITE, C_BLACK, DTEXT_MIDDLE, DTEXT_MIDDLE, F3, -1); // F3
    dtext_opt(32 + 66 * 3, 214, C_WHITE, C_BLACK, DTEXT_MIDDLE, DTEXT_MIDDLE, F4, -1); // F4
    dtext_opt(32 + 66 * 4, 214, C_WHITE, C_BLACK, DTEXT_MIDDLE, DTEXT_MIDDLE, F5, -1); // F5
    dtext_opt(32 + 66 * 5, 214, C_WHITE, C_BLACK, DTEXT_MIDDLE, DTEXT_MIDDLE, F6, -1); // F6
}

#define TOPLEFT_ADDR_X 8
#define TOPLEFT_ADDR_Y 26
#define TOPLEFT_LSN_X  40
#define TOPLEFT_LSN_Y  16
#define TOPLEFT_VAL_X  37
#define TOPLEFT_VAL_Y  26

#define ADDR_OFFSET  (7 + 4)
#define LSN_OFFSET   (5 + 9)
#define VAL_OFFSET_X (5 * 2 + 1 + 3)
#define VAL_OFFSET_Y (7 + 4)


void drawMemory(uint8_t* memory, uint16_t startAddress, uint16_t programCounter) {
    startAddress = startAddress & 0xFFF0;

    for (int addrX = 0; addrX < 0x10 + 1; addrX++) {
        for (int addrY = 0; addrY < 0x10 + 1; addrY++) {
            if (addrX == 0 && addrY == 0) continue;
            if ((startAddress + ((addrY - 1) << 4)) > 0xFFFF) {
                drect(TOPLEFT_ADDR_X, 
                    TOPLEFT_ADDR_Y + ADDR_OFFSET * (addrY - 1), 
                    TOPLEFT_ADDR_X + 4 * (5 + 1),
                    TOPLEFT_ADDR_Y + ADDR_OFFSET * (addrY - 1) + 7,
                    C_BLACK
                );

                if (addrX > 0)
                drect(TOPLEFT_VAL_X + VAL_OFFSET_X * (addrX - 1), 
                    TOPLEFT_VAL_Y + VAL_OFFSET_Y * (addrY - 1) - 1,
                    TOPLEFT_VAL_X + VAL_OFFSET_X * (addrX - 1) + 2 * 5 + 1,
                    TOPLEFT_VAL_Y + VAL_OFFSET_Y * (addrY - 1) + 7,
                    C_BLACK
                );

                continue;
            }
            if (addrX == 0) { // Address
                drect(TOPLEFT_ADDR_X, 
                    TOPLEFT_ADDR_Y + ADDR_OFFSET * (addrY - 1), 
                    TOPLEFT_ADDR_X + 4 * (5 + 1),
                    TOPLEFT_ADDR_Y + ADDR_OFFSET * (addrY - 1) + 7,
                    C_BLACK
                );
                dprint(TOPLEFT_ADDR_X,
                    TOPLEFT_ADDR_Y + ADDR_OFFSET * (addrY - 1),
                    C_WHITE,
                    "%04X", ((startAddress + ((addrY - 1) << 4)) > 0xFFFF) ?
                    0x0000 : startAddress + ((addrY - 1) << 4)
                );
            }
            else { // Not Address
                if (addrY == 0) { // Least Significant Nibble
                    dprint(TOPLEFT_LSN_X + LSN_OFFSET * (addrX - 1),
                        TOPLEFT_LSN_Y,
                         C_WHITE, 
                        "%0X", addrX - 1
                    );
                }
                else { // Value
                    drect(TOPLEFT_VAL_X + VAL_OFFSET_X * (addrX - 1) - 1, 
                        TOPLEFT_VAL_Y + VAL_OFFSET_Y * (addrY - 1) - 1,
                        TOPLEFT_VAL_X + VAL_OFFSET_X * (addrX - 1) + 2 * 5 + 1,
                        TOPLEFT_VAL_Y + VAL_OFFSET_Y * (addrY - 1) + 7,
                        (startAddress + ((addrY - 1) << 4) + addrX - 1) ? C_WHITE : C_BLACK
                    );
                    dprint_opt(TOPLEFT_VAL_X + VAL_OFFSET_X * (addrX - 1),
                        TOPLEFT_VAL_Y + VAL_OFFSET_Y * (addrY - 1), 
                        (startAddress + ((addrY - 1) << 4) + addrX - 1) ? C_BLACK : C_WHITE,
                        (startAddress + ((addrY - 1) << 4) + addrX - 1) ? C_WHITE : C_BLACK,
                        DTEXT_LEFT,
                        DTEXT_TOP,
                        "%02X", 
                        ((startAddress + ((addrY - 1) << 4) + addrX - 1) > 0XFFFF) ? 
                            0x00 : memory[startAddress + ((addrY - 1) << 4) + addrX - 1]
                    );
                }
            }
        }
    }
}

void drawMemoryMode(bool EDIT) {
    dprint_opt(238, 5, (EDIT) ? C_RED : C_WHITE, C_BLACK, DTEXT_LEFT, DTEXT_TOP, "%s", (EDIT) ? "EDIT" : "VIEW");
}

#define WORD_TO_BINARY(word) \
  (word & 0x8000 ? '1' : '0'), \
  (word & 0x4000 ? '1' : '0'), \
  (word & 0x2000 ? '1' : '0'), \
  (word & 0x1000 ? '1' : '0'), \
  (word & 0x0800 ? '1' : '0'), \
  (word & 0x0400 ? '1' : '0'), \
  (word & 0x0200 ? '1' : '0'), \
  (word & 0x0100 ? '1' : '0'), \
  (word & 0x0080 ? '1' : '0'), \
  (word & 0x0040 ? '1' : '0'), \
  (word & 0x0020 ? '1' : '0'), \
  (word & 0x0010 ? '1' : '0'), \
  (word & 0x0008 ? '1' : '0'), \
  (word & 0x0004 ? '1' : '0'), \
  (word & 0x0002 ? '1' : '0'), \
  (word & 0x0001 ? '1' : '0')

#define BYTE_TO_BINARY(byte) \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define WORD_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"

#define Y_OFFSET 8

#define PC_POSX 271
#define PC_POSY 16

#define P_POSX  271
#define P_POSY  35

#define SP_POSX 271
#define SP_POSY 54

#define A_POSX  325
#define A_POSY  54

#define X_POSX  271
#define X_POSY  73

#define Y_POSX  325
#define Y_POSY  73

void drawRegisters(mos6502 mpu) {
    // Program Counter
    dprint_opt(PC_POSX,
        PC_POSY,
        C_WHITE,
        C_BLACK,
        DTEXT_LEFT,
        DTEXT_TOP,
        "PC %04X",
        mpu.GetPC(),
        WORD_TO_BINARY(mpu.GetPC())
    );
    dprint_opt(PC_POSX,
        PC_POSY + Y_OFFSET,
        C_WHITE,
        C_BLACK,
        DTEXT_LEFT,
        DTEXT_TOP,
        WORD_TO_BINARY_PATTERN,
        WORD_TO_BINARY(mpu.GetPC())
    );

    // Status Register
    dprint_opt(P_POSX,
        P_POSY,
        C_WHITE,
        C_BLACK,
        DTEXT_LEFT,
        DTEXT_TOP,
        "P  %02X",
        mpu.GetP()
    );
    dprint_opt(P_POSX,
        P_POSY + Y_OFFSET,
        C_WHITE,
        C_BLACK,
        DTEXT_LEFT,
        DTEXT_TOP,
        BYTE_TO_BINARY_PATTERN,
        BYTE_TO_BINARY(mpu.GetP())
    );

    // Stack Pointer
    dprint_opt(SP_POSX,
        SP_POSY,
        C_WHITE,
        C_BLACK,
        DTEXT_LEFT,
        DTEXT_TOP,
        "SP %02X",
        mpu.GetS()
    );
    dprint_opt(SP_POSX,
        SP_POSY + Y_OFFSET,
        C_WHITE,
        C_BLACK,
        DTEXT_LEFT,
        DTEXT_TOP,
        BYTE_TO_BINARY_PATTERN,
        BYTE_TO_BINARY(mpu.GetS())
    );

    // Accumulator
    dprint_opt(A_POSX,
        A_POSY,
        C_WHITE,
        C_BLACK,
        DTEXT_LEFT,
        DTEXT_TOP,
        "A  %02X",
        mpu.GetA()
    );
    dprint_opt(A_POSX,
        A_POSY + Y_OFFSET,
        C_WHITE,
        C_BLACK,
        DTEXT_LEFT,
        DTEXT_TOP,
        BYTE_TO_BINARY_PATTERN,
        BYTE_TO_BINARY(mpu.GetA())
    );

    // X Register
    dprint_opt(X_POSX,
        X_POSY,
        C_WHITE,
        C_BLACK,
        DTEXT_LEFT,
        DTEXT_TOP,
        "X  %02X",
        mpu.GetX()
    );
    dprint_opt(X_POSX,
        X_POSY + Y_OFFSET,
        C_WHITE,
        C_BLACK,
        DTEXT_LEFT,
        DTEXT_TOP,
        BYTE_TO_BINARY_PATTERN,
        BYTE_TO_BINARY(mpu.GetX())
    );

    // Y Register
    dprint_opt(Y_POSX,
        Y_POSY,
        C_WHITE,
        C_BLACK,
        DTEXT_LEFT,
        DTEXT_TOP,
        "Y  %02X",
        mpu.GetY()
    );
    dprint_opt(Y_POSX,
        Y_POSY + Y_OFFSET,
        C_WHITE,
        C_BLACK,
        DTEXT_LEFT,
        DTEXT_TOP,
        BYTE_TO_BINARY_PATTERN,
        BYTE_TO_BINARY(mpu.GetY())
    );
}


void drawDisassembler(uint8_t* memory, uint16_t address, uint16_t programCounter) {
    
}


// 148x87 window. Top left 124, 65
image_t *drawWindow(const char* title) {
    image_t *temporary = image_copy_alloc(image_sub(image_create_vram(), 123, 53, 150, 100), IMAGE_RGB565);
    drect_border(123, 53, 272, 152, C_BLACK, 1, C_WHITE);
    dline(123, 64, 271, 64, C_WHITE);
    dtext(126, 56, C_WHITE, title);
    return temporary;
}

void freeWindow(image_t **temporary) {
    image_copy(*temporary, image_sub(image_create_vram(), 123, 53, 150, 100), true);
	image_free(*temporary);
    *temporary = NULL;
}
