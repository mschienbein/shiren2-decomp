#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 kind; s32 x; s32 y; s32 z; } Desc800489E0;
typedef struct { s32 a; s32 b; } Pair800489E0;
typedef struct {
    u8 pad0[0x28];
    s32 field_28;
    s32 field_2C;
    s16 field_30;
    u8 pad32[0x22];
    u8 field_54[0x10];
    u8 field_64[0x10];
} Obj800489E0;

extern Pair800489E0 D_8014AADC;
u8 *func_8006A810(void *dst, s32 value, s32 size);
void func_80095E58(void *dst, Obj800489E0 *obj, Pair800489E0 pair);
void func_800486A4(void *dst, Desc800489E0 *desc, void *src);

void func_800489E0(Obj800489E0 *obj) {
    Desc800489E0 copy;
    Desc800489E0 desc;

    func_8006A810(&desc, 0, sizeof(desc));
    desc.kind = 2;
    desc.x = obj->field_30 - 8;
    desc.y = obj->field_28 - 3;
    desc.z = obj->field_2C + 4;
    copy = desc;
    func_80095E58(obj->field_64, obj, D_8014AADC);
    func_800486A4(obj->field_54, &copy, obj->field_64);
}
