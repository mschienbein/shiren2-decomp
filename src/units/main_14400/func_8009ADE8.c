#include "common.h"

typedef unsigned char u8;

typedef struct {
    unsigned char pad0[0x78];
    s32 field_78;
    s32 field_7C;
    unsigned char pad80[0x88 - 0x80];
    s32 field_88;
} Obj;

typedef struct Desc Desc;

extern Desc D_80138D68;
void func_8009AF4C(Obj *obj, s32 arg);
void func_8009AE50(Obj *obj, u8 *text);
void func_8009543C(Obj *obj, Desc *desc);

void func_8009ADE8(void *p, s32 length, u8 *text, s32 mode)
{
    Obj *obj = p;

    obj->field_7C = length;
    func_8009AF4C(obj, 0);
    obj->field_78 = 0;
    obj->field_88 = mode;
    func_8009AE50(obj, text);
    func_8009543C(obj, &D_80138D68);
}
