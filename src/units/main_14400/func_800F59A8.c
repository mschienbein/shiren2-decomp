#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 pad0[0xA];
    u8 field_A;
    u8 padB[0x1F - 0xB];
    u8 field_1F;
    u8 pad20[4];
    const void *vtable_24;
} Obj800F59A8;
extern const unsigned char D_80159850[112];
Obj800F59A8 *func_800F4760(Obj800F59A8 *obj);

Obj800F59A8 *func_800F59A8(Obj800F59A8 *obj) {
    func_800F4760(obj);
    obj->vtable_24 = D_80159850;
    obj->field_A = 0x10;
    obj->field_1F = 0x10;
    return obj;
}
