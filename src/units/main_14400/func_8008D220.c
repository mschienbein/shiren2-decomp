#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { u8 used; u8 pad[0x77]; } Slot;
typedef struct { u8 pad[0x100]; Slot slots[8]; u8 pad4C0[4]; s32 count; } S;
void func_8008C950(Slot *);
s32 func_8008D220(S *s) {
    s32 i;
    Slot *slot;
    Slot *slots = s->slots;
    for (i = 0; i < 8; i++) {
        slot = &slots[i];
        if (slot->used == 0) {
            func_8008C950(slot);
            slot->used = 1;
            s->count++;
            break;
        }
    }
    if (i >= 8) i = -1;
    return i;
}
