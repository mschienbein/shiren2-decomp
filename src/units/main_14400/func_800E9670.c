#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x98]; s16 delta; u8 pad9A[2]; void *(*get)(void *self); } VTable800E9670;
typedef struct { u8 pad0[0x24]; VTable800E9670 *vtable; } Obj800E9670;
s32 func_800CF1C8(void *list, u8 kind);
s32 func_800A63FC(void *obj, void *position);
s32 func_800E9670(Obj800E9670 *obj, void *position) {
    void *item = obj->vtable->get((u8 *)obj + obj->vtable->delta);
    if (item != 0 && func_800CF1C8(item, 0x85) > 0) {
        return 1;
    }
    return func_800A63FC(obj, position);
}
