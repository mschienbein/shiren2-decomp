#include "common.h"
typedef struct {
    unsigned char pad0[0xA]; unsigned char field_A;
    unsigned char padB[0x14]; unsigned char field_1F;
    unsigned char pad20[4]; void *field_24; s32 field_28;
} Object;
extern void *func_800A38FC(s32 size);
extern void *func_800F4760(Object *obj);
extern unsigned char D_801494D8[];
Object *func_800F5630(void) {
    Object *obj = func_800A38FC(0x2C);
    func_800F4760(obj);
    obj->field_24 = D_801494D8;
    obj->field_A = 14;
    obj->field_1F = 14;
    return obj;
}
