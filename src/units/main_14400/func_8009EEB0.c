#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x4C];
    void *field_4C;
} Obj8009EEB0;

extern Obj8009EEB0 D_801425F0;
extern u8 D_80149E50[];

Obj8009EEB0 *func_800953C0(Obj8009EEB0 *obj);

void func_8009EEB0(void) {
    Obj8009EEB0 *obj = &D_801425F0;

    func_800953C0(obj);
    obj->field_4C = D_80149E50;
}
