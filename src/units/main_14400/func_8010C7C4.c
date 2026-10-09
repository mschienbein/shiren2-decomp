#include "common.h"

typedef unsigned char u8;

/* Partial view matching func_8010BEC4's item record (owner count at 0x0F). */
typedef struct {
    u8 pad0;
    u8 f_1;
    u8 pad2[0xD];
    u8 count;
    u8 items[16];
} S;

extern s32 func_8010BEC4(S *s, u8 c);

/* Item vtable slot 1: true when the item carries effect 0xF7. */
s32 func_8010C7C4(S *item) {
    return (u8)func_8010BEC4(item, 0xF7) != 0;
}
