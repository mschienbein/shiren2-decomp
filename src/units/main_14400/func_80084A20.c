#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct {
    u16 a;
    u16 b;
} Pair80084A20;

extern Pair80084A20 D_801BF258[];
extern s32 D_801BF2C0;
extern s32 D_801BF2C4;
/* Read the low halfword of the mode word, rather than declaring D_8013E902. */
extern s32 D_8013E900;

void func_80084A20(void) {
    D_801BF258[D_801BF2C4].a = (u16)D_8013E900;
    D_801BF258[D_801BF2C4].b = (u16)D_801BF2C0;
    D_801BF2C4++;
}
