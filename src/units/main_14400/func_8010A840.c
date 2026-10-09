#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x24];
    const void *vtable_24;
    u8 pad28[0x8C];
    const void *vtable_B4;
} Obj8010A840;

extern const unsigned char D_80159130[];
extern const unsigned char D_8015C9F0[];
extern const unsigned char D_80159150[];

s32 func_800EE598(Obj8010A840 *obj);
void func_800E016C(Obj8010A840 *obj, s32 flags);
void func_800A3918(void *obj);

void func_8010A840(Obj8010A840 *obj, s32 flags)
{
    obj->vtable_B4 = D_80159130;
    obj->vtable_24 = D_8015C9F0;
    func_800EE598(obj);
    obj->vtable_B4 = D_80159130;
    obj->vtable_24 = D_80159150;
    func_800E016C(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
