#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 x0; void *x4; } S;
extern u8 D_80158658[];
void *func_800DA904(void *obj, s32 kind, u8 *params);
S *func_800DC968(S *s, u8 *arg) {
    func_800DA904(s, 0x1A, arg);
    s->x4 = D_80158658;
    return s;
}
