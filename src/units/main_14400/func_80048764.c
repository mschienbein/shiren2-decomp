#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad_00[0xC]; s32 field_0C; } S;
extern void func_800826FC(s32 index);
extern void func_800828FC(void);

void func_80048764(S *s) {
    if (s->field_0C >= 0) {
        func_800826FC(s->field_0C);
        func_800828FC();
    }
}
