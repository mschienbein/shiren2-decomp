#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Partial view matching func_8010BEC4's item record. */
typedef struct {
    u8 pad0;
    u8 f_1;
    u8 pad2[0xD];
    u8 count;
    u8 items[16];
} S;

extern u16 func_800E08B0(void *obj);
extern u16 func_800E08F0(void *obj);
extern s32 func_8010BEC4(S *s, u8 c);

/* ODD_C: the byte predicate preserves a zero-based nonzero test across inlining. */
static inline u8 nonzero(u32 value) { return value != 0; }
s32 func_8010EC5C(S *item, void *obj) {
    s32 result = 0;
    if (func_800E08B0(obj) == func_800E08F0(obj)) {
        u8 count = func_8010BEC4(item, 5);
        result = nonzero(count);
    }
    return result;
}
