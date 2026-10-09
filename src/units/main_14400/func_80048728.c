#include "common.h"

/* Partial view: only the handle at 0x0C is used. */
typedef struct Obj80048728 {
    u32 opaque_00[3];
    s32 handle_0C;
} Obj80048728;

void func_80081D84(s32 handle);

void func_80048728(Obj80048728 *obj)
{
    if (obj->handle_0C >= 0) {
        func_80081D84(obj->handle_0C);
        obj->handle_0C = -1;
    }
}
