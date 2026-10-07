#include "common.h"

typedef struct {
    char pad0[0x2E];
    unsigned short field_2E;
    char pad30[0xC];
    unsigned char field_3C;
} Obj;

u32 func_800E0EAC(Obj *obj) {
    if (obj->field_3C != 0) {
        return (obj->field_2E + 1U) >> 1;
    }
    return obj->field_2E;
}
