#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { s32 field_0; u8 pad4[0x8]; s32 field_C; } Src800A34B8;
typedef struct { s32 a; s32 b; } Pair800A34B8;
Pair800A34B8 *func_800A34B8(Pair800A34B8 *out, Src800A34B8 *src) {
    s32 a = src->field_0;
    s32 b = src->field_C;
    out->a = a;
    out->b = b;
    return out;
}
