#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x, y; } Point;
typedef union { s32 word; u8 bytes[4]; } Word;
typedef struct { u8 bytes[4]; } Dims;
typedef struct Desc Desc;
typedef struct { u8 storage[12]; } Entry;
typedef struct {
    u8 pad0[0x3C]; Point field3C;
    u8 pad44[0x10]; Word field54; Word field58;
    Entry field5C[30];
} Object;
extern Desc D_80142894;
extern u8 func_800CCC44(u8 kind, Entry *out, u8 first, u8 count);
extern u8 *func_8006A810(u8 *dst, s32 value, s32 count);
extern void func_8009D910(Object *obj, Desc *src, Dims *dims);

void func_8009EEF0(Object *self, u8 kind, s32 index)
{
    Dims copy;
    Dims dims;
    Point position;
    self->field54.word = kind;
    self->field58.word = func_800CCC44(self->field54.bytes[3], self->field5C, 0, 30);
    func_8006A810(dims.bytes, 0, 4);
    dims.bytes[0] = 10;
    dims.bytes[1] = 1;
    dims.bytes[2] = 28;
    dims.bytes[3] = self->field58.bytes[3];
    copy = dims;
    func_8009D910(self, &D_80142894, &copy);
    if (index >= self->field58.word) {
        index = 0;
    }
    position.y = index / 10;
    position.x = index % 10;
    self->field3C = position;
}
