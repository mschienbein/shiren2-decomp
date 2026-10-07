#include "common.h"

typedef signed char s8;

typedef struct {
    unsigned char pad0[0x56];
    s8 field_56;
} Obj800E2408;

s32 func_800E2408(Obj800E2408 *obj) {
    return obj->field_56;
}
