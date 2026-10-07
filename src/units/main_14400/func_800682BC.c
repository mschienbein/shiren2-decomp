#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 used; u8 pad1[7]; s32 field_8; s32 field_C; s32 field_10; } Slot800682BC;
extern Slot800682BC *D_801D2560;
Slot800682BC *func_800682BC(void) {
    Slot800682BC *slot;
    u32 i;
    for (i = 0, slot = D_801D2560; i < 0xC6; i++, slot++) {
        if (slot->used == 0) {
            slot->used = 1;
            slot->field_8 = 0;
            slot->field_10 = 0;
            slot->field_C = 0;
            return slot;
        }
    }
    return 0;
}
