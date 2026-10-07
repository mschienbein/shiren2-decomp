#include "common.h"

typedef struct {
    unsigned char pad0[0x104];
    void *field_104;
} Obj;

extern Obj *D_801476B8;

s32 func_800E20CC(void *arg0) {
    s32 result = 0;
    if (D_801476B8->field_104 != 0) {
        result = arg0 == D_801476B8->field_104;
    }
    return result;
}
