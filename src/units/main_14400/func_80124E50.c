#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad_00[0x90]; short delta_90; short index_92; s32 (*method_94)(void *, s32, s32, u8, s32); } Methods;
typedef struct { u8 pad_00[0x1E]; u8 field_1E; u8 pad_1F[5]; Methods *field_24; } Obj;
/* 80116028..80116050 supplies all seven pointers; only obj is consumed here. */
s32 func_80124E50(void *self, void *owner, void *from, void *to, void *value, Obj *obj, void *source)
{
    if (obj != 0 && (obj->field_1E & 0x7C) != 0) {
        obj->field_24->method_94((u8 *)obj + obj->field_24->delta_90, 0, 0x11, 0xFE, -1);
    }
    return 1;
}
