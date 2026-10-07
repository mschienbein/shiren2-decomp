#include "common.h"

/* +0xC4 holds the item pointer stored by func_800DEA54 (getter results) and the constructor. */
typedef struct { char pad0[0xC4]; void *field_C4; } Obj;
extern s32 func_800DDCA8(Obj *);

s32 func_800DEAF4(Obj *o) {
    s32 notReady = func_800DDCA8(o) != 1;
    if (notReady) {
        return 0;
    }
    return o->field_C4 != 0;
}
