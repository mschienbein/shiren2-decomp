#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad00;
    u8 kind;
    u8 pad02[0xB];
    u8 field_0D;
    u8 field_0E;
} Item801199E0;

/* Callers (0x80119A9C, 0x80119E48) supply their receiver in a0; it is unused here. */
s32 func_801199E0(void *arg0, Item801199E0 *item, u8 arg2, u8 arg3) {
    s32 result = 0;

    if (item->kind == 0xF1 && item->field_0D == arg2 && item->field_0E == arg3) {
        result = 1;
    } else if (item->kind == 0xF2 && arg2 == 0x29 && (item->field_0D & 0x7F) == arg3) {
        result = 1;
    }
    return result;
}
