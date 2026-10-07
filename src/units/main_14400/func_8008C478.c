#include "common.h"
typedef unsigned char u8;
typedef struct { char pad0[2]; u8 flags; char pad3[0x75]; } Entry;
typedef struct { char pad[0x100]; Entry entries[8]; } Table;
extern s32 D_8013FEE0;
extern Table *D_8013FEE4;
void func_8008C478(s32 i, u8 on) {
    if (D_8013FEE0) {
        Entry *e = &D_8013FEE4->entries[i];
        if (on) e->flags |= 1;
        else e->flags &= ~1;
    }
}
