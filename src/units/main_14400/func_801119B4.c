#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;
typedef struct Item Item;

extern u16 D_80157698[];
extern u8 D_801576FC[];
u8 func_800AE98C(Item *item);
s32 func_800AC584(u16 id);
s32 func_801119B4(Item *item) {
    u8 index = func_800AE98C(item);
    return func_800AC584(D_80157698[index]) * D_801576FC[index] / 100;
}
