#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x10]; void *field_10; } Obj800C4E04;
typedef struct { s32 x; s32 y; } Position;
s32 func_800C4F18(void *self, void *from, void *to);
s32 func_800C4E04(Obj800C4E04 *obj, Position *from, Position *to) {
    return func_800C4F18(obj->field_10, from, to);
}
