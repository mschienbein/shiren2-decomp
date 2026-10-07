#include "common.h"

typedef struct {
    char pad0[0x2CC];
    s32 field_2CC;
} Obj800479C4;

s32 func_800479C4(Obj800479C4 *obj) {
    return obj->field_2CC;
}
