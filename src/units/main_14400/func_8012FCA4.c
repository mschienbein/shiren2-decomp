#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

typedef struct {
    u16 field_0;
    u16 field_2;
    char pad4[4];
    char field_8[0x20];
    void *field_28;
    union {
        s32 word;
        u8 bytes[4];
    } field_2C;
} Obj;

extern u32 func_800340F0(void *addr);

Gfx *func_8012FCA4(Obj *obj, s32 arg1, Gfx *gfx) {
    s32 high = arg1 >> 8;
    Gfx *next = gfx + 1;

    gfx->w0 = 0x0B000020;
    gfx->w1 = func_800340F0(obj->field_8);
    {
        Gfx *g = next++;

        g->w0 = 0x0E000000 | (obj->field_2C.bytes[3] << 16) | obj->field_2;
        g->w1 = (high << 24) | (func_800340F0(obj->field_28) & 0xFFFFFF);
    }

    obj->field_2C.word = 0;
    return next;
}
