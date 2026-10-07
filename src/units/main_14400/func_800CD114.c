#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s16 delta; s16 index; void *fn; } VEntry;
typedef struct { s32 f_0; VEntry *vt; } Obj;
typedef s32 (*MaxFn)(void *self);
typedef s32 (*GetFn)(void *self);
typedef void (*SetFn)(void *self, s32 v);
s32 func_800CD114(Obj *obj, s32 delta) {
    s32 v = delta + ((GetFn)obj->vt[2].fn)((u8 *)obj + obj->vt[2].delta);
    if (v < 0) {
        ((SetFn)obj->vt[3].fn)((u8 *)obj + obj->vt[3].delta, 0);
        return v;
    }
    if ((u32)((MaxFn)obj->vt[1].fn)((u8 *)obj + obj->vt[1].delta) < (u32)v) {
        ((SetFn)obj->vt[3].fn)((u8 *)obj + obj->vt[3].delta,
            ((MaxFn)obj->vt[1].fn)((u8 *)obj + obj->vt[1].delta));
        return v - ((MaxFn)obj->vt[1].fn)((u8 *)obj + obj->vt[1].delta);
    }
    ((SetFn)obj->vt[3].fn)((u8 *)obj + obj->vt[3].delta, v);
    return 0;
}
