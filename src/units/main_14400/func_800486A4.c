#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Point;

/* +0x8 holds the caller's object; func_80048650 dispatches through its vtable. */
typedef struct {
    Point pos;
    void *target;
    s32 handle;
} Obj;

typedef struct {
    Point pos;
    s32 field_8;
    s32 field_C;
} Src;

void func_80048688(void *arg);
void func_80048728(Obj *obj);
s32 func_80081C18(s32 arg0, s32 arg1, s32 y, s32 x, void (*callback)(void *), void *arg);

void func_800486A4(Obj *obj, Src *src, void *target) {
    if (obj->handle >= 0) {
        func_80048728(obj);
    }
    obj->target = target;
    obj->pos = src->pos;
    obj->handle = func_80081C18(src->field_C, src->field_8, obj->pos.y, obj->pos.x, func_80048688, obj);
}
