#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct {
    s16 id;
} Def80053438;

typedef struct {
    Def80053438 *def;
    u8 pad4[0x8];
} Slot80053438;

extern Slot80053438 D_801616D0[2];
extern void func_80053590(Slot80053438 *record);

void func_80053438(s32 index) {
    Slot80053438 *slot = D_801616D0;
    s32 i;

    for (i = 1; i != -1; i--, slot++) {
        if (slot->def->id == (s16)index) {
            func_80053590(slot);
            return;
        }
    }
}
