#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 left; s32 top; s32 right; s32 bottom; } Rect;
Rect *func_800A324C(Rect *buf, Rect *a, Rect *b);
s32 func_800A34D0(Rect *buf);
s32 func_800A32D8(Rect *a, Rect *b) {
    Rect buf;

    func_800A324C(&buf, a, b);
    return func_800A34D0(&buf);
}
