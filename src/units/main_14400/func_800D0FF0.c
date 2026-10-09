#include "common.h"
typedef unsigned char u8;
extern void func_800D10F4(void **slots);
extern u8 *func_800CD9EC(void *self, u8 value);

/* True when any of the three filled slots holds an entry of kind 0x11. */
s32 func_800D0FF0(void) {
    void *entries[3];
    s32 i;
    func_800D10F4(entries);
    i = 0;
    do {
        if (entries[i] != 0 && func_800CD9EC(entries[i], 0x11) != 0) {
            return 1;
        }
    } while (++i < 3);
    return 0;
}
