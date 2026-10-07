#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0x84]; s32 field_84; } Obj80041C54;
extern Obj80041C54 *D_801476B8;

s32 func_80041C54(void) {
    return D_801476B8->field_84;
}
