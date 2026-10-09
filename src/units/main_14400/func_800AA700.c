#include "common.h"
typedef unsigned char u8;
extern void func_800AA7CC(u8 *, u8 *);
extern s32 func_800AA48C(u8, u8);

void func_800AA700(u8 *first, u8 *second, u8 *excluded) {
    s32 remaining = 10;
    for (;;) {
        u8 *entry;
        if (--remaining != -1) {
            func_800AA7CC(first, second);
            if (*first == 0) return;
            if (func_800AA48C(*first, *second)) continue;
            if (excluded == 0) return;
            entry = excluded;
            while (*entry != 0) {
                if (*entry == *first) break;
                entry++;
            }
            if (*entry != 0) continue;
            return;
        }
        *first = 0;
        return;
    }
}
