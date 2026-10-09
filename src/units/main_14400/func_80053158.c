#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    void *data;
    s16 field_4;
    u8 pad6[6];
} Channel80053158;

extern Channel80053158 D_801616F4;

extern void func_800535D0(s32 mode, Channel80053158 *channel, s32 level, u8 flag);

void func_80053158(u8 level, u8 flag) {
    func_800535D0(1, &D_801616F4, level, flag);
}
