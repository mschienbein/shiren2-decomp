#include "common.h"
typedef struct { unsigned char color[4], state[0x20]; float values[8]; unsigned char active, pad45[3]; } SavedState;
extern SavedState D_80138BF4;

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;


void func_800265E0(void *dst, s32 size);

void func_80046270(void) {
    func_800265E0(&D_80138BF4, 0x48);
}
