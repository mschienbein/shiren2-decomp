#include "common.h"
extern void func_80052494(short);
/* The overlay supplies a word; ROM 0x18B0C/0x18B18 narrows here. */
void func_80045AC8(s32 x) {
    short id = x;
    func_80052494(id);
}
