#include "common.h"

typedef unsigned short u16;

extern u16 func_80083850(s32 i);
extern s32 func_8005DE90(char *text, s32 color);

void func_80053D20(s32 index, char *text) {
    func_8005DE90(text, func_80083850(index));
}
