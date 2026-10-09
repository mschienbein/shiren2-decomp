#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct S S;

typedef struct {
    u8 pad0[0x2];
    u8 flags2;
} Item80110170;

extern s32 func_8010BEC4(S *s, u8 c);

/* Vtable slot 3 of D_8015D518: s32 (*)(self, kind), like its peers. */
s32 func_80110170(Item80110170 *item, s32 kind) {
    if (kind == 2 || kind == 0xB) {
        return 1;
    }
    if (kind == 0 && !(item->flags2 & 4)) {
        return 1;
    }
    if (kind == 1 && (item->flags2 & 4)) {
        return 1;
    }
    if (kind == 9 && (u8)func_8010BEC4((S *)item, 0x42)) {
        return 1;
    }
    if (kind == 8 && (u8)func_8010BEC4((S *)item, 0x3F)) {
        return 1;
    }
    if (kind == 0x21 && (u8)func_8010BEC4((S *)item, 0x78)) {
        return 1;
    }
    return 0;
}
