#include "common.h"

typedef struct {
    unsigned char pad_00[0x0A];
    unsigned char kind_0A;
    unsigned char pad_0B[0x14];
    unsigned char kind_1F;
    unsigned char pad_20[4];
    const void *vtable_24;
    unsigned char pad_28[4];
    unsigned char state_2C;
} Object;
extern const unsigned char D_80159938[120];
extern void *func_800F4760(Object *object);

Object *func_800F5E30(Object *object)
{
    func_800F4760(object);
    object->vtable_24 = D_80159938;
    object->kind_0A = 0x13;
    object->kind_1F = 0x13;
    object->state_2C = 0;
    return object;
}
