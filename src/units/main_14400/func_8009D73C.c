#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad[0x4C]; const void *x4C; } S;
extern const unsigned char D_80151E38[144];
void func_800D8FA8(void *object);
void func_8009D73C(S *s, s32 flags) {
    s->x4C = D_80151E38;
    if (flags & 1) {
        func_800D8FA8(s);
    }
}
