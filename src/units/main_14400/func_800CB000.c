#include "common.h"

#define GOLD_MAX 0x22550FF

typedef struct {
    unsigned char pad0[0x18];
    u32 field_18;
} Obj;

void func_800CB000(Obj *obj, u32 amount)
{
    u32 current = obj->field_18;

    if (GOLD_MAX - current < amount) {
        obj->field_18 = GOLD_MAX;
        return;
    }
    obj->field_18 = current + amount;
}
