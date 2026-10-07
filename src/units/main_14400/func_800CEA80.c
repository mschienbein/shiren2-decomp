#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0xC];
    u8 field_C;
} Obj;

/* Collection slot +0xC returns a full int capacity, including for byte-backed lists. */
s32 func_800CEA80(Obj *obj) {
    return obj->field_C;
}
