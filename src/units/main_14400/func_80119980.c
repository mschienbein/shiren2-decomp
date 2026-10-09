#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[8];
    const void *field_8;
} Obj;

extern const unsigned char D_8015E410[80];
/* Returns the input object through this TU's partial view. */
Obj *func_80112D20(Obj *, s32);

Obj *func_80119980(Obj *obj) {
    func_80112D20(obj, 0x17);
    obj->field_8 = D_8015E410;
    return obj;
}
