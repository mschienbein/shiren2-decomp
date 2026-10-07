#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef short s16;

typedef struct {
    s32 kind;
    s32 x;
    s32 y;
    s32 z;
} Rect_8009E414;

typedef struct {
    s32 word0;
    s32 word4;
} Pair_8009E414;

typedef struct {
    u8 pad0[0x28];
    s32 field_28;
    s32 field_2C;
    s16 field_30;
    u8 pad32[0x58 - 0x32];
    u8 field_58[0x10];
    u8 field_68[0x10];
} Obj_8009E414;

extern Pair_8009E414 D_80152D5C;
extern u8 *func_8006A810(void *dst, s32 value, s32 size);
extern void func_80095E58(void *out, Obj_8009E414 *obj, Pair_8009E414 pair);
extern void func_800486A4(void *dst, Rect_8009E414 *rect, void *src);

void func_8009E414(Obj_8009E414 *obj) {
    Rect_8009E414 copy;
    Rect_8009E414 rect;

    func_8006A810(&rect, 0, sizeof(rect));
    rect.kind = 2;
    rect.x = obj->field_30 - 6;
    rect.y = obj->field_28 - 3;
    rect.z = obj->field_2C + 2;
    copy = rect;
    func_80095E58(obj->field_68, obj, D_80152D5C);
    func_800486A4(obj->field_58, &copy, obj->field_68);
}
