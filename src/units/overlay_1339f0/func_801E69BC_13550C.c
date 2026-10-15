#include "func_801E69BC_13550C/types.h"

void func_801E69BC_13550C(u16 value, char *text) {
    if (value > 999) {
        value = 999;
    }
    text[0] = (u16)(value / 100) + 15 + (value >= 100);
    text[1] = (u16)((u16)(value % 100) / 10) + 15 + (value >= 10);
    text[2] = value % 10 + 16;
}
