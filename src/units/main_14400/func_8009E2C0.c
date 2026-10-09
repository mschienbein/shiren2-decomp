#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0xF]; u8 fieldF; } Data;
typedef struct { u8 pad0[0x54]; Data *field54; u8 pad58[0x20]; short field78; } Object;
typedef struct { u8 width, height, field2, field3; } Dims;
typedef struct Descriptor Descriptor;
extern Descriptor D_80139028;
extern u8 *func_8006A810(u8 *dst, s32 value, s32 count);
extern void func_8009D910(Object *object, Descriptor *descriptor, Dims *dimensions);
void func_8009E2C0(Object *object, Data *value, s32 mode) {
    Dims copy, dims;
    object->field54 = value;
    object->field78 = mode;
    func_8006A810((u8 *)&dims, 0, 4);
    dims.width = 8;
    dims.height = 1;
    dims.field3 = object->field54->fieldF;
    copy = dims;
    func_8009D910(object, &D_80139028, &copy);
}
