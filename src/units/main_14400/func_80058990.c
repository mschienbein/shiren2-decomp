#include "common.h"
typedef struct { unsigned short field0; unsigned char field2, field3; } Entry;
extern short D_80163110[4];
extern Entry D_801D2C10[];
void func_80058990(void) {
    s32 i;
    short *out;
    Entry *entry;
    i = 0;
    for (;;) {
        s32 before_end = ++i < 4;
        if (!before_end) break;
    }
    i = 0;
    out = D_80163110;
    entry = D_801D2C10;
    for (; i < 4; i++, entry++) {
        if (!entry->field3 && (entry->field0 & 0x1F07) == 5) *out++ = i;
    }
    while (out < &D_80163110[4]) *out++ = 0x80;
}
