#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad0[0x50];
    s16 field_50;
    u8 pad52[0x5A - 0x52];
    s16 field_5A;
    u8 pad5C[0x60 - 0x5C];
} Entry80056D00;

extern Entry80056D00 D_801D40DC[];
void func_80056504(s32 index);

void func_80056D00(Entry80056D00 *entry) {
    entry->field_50 = 0;
    entry->field_5A = 0;
    func_80056504(entry - D_801D40DC);
}
