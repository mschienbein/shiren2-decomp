#include "common.h"
typedef unsigned char u8;
typedef short s16;
typedef struct { u8 unk0; u8 pad1[0x2B]; s16 unk2C; } Ent;
extern Ent D_801A79E8[];
void func_800765AC(void) {
    s32 i;
    for (i = 0; i < 30; i++) {
        D_801A79E8[i].unk0 = 0;
        D_801A79E8[i].unk2C = -1;
    }
}
