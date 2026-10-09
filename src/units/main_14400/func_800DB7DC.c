#include "common.h"

/* Collection/item action link (func_800DA8A0 copies the source link to +8). */
typedef struct {
    void *collection;
    void *item;
} Link;

typedef struct {
    short id;
    short pad2;
    void *vtable;
    Link source;
    Link target;
} Obj;

extern unsigned char D_801584A8[];
Obj *func_800DA8A0(Obj *self, s32 kind, Link *source);
Link *func_800D0180(Link *link);

Obj *func_800DB7DC(Obj *obj, Link *source, Link *target)
{
    func_800DA8A0(obj, 0x11, source);
    obj->vtable = D_801584A8;
    func_800D0180(&obj->target);
    obj->target = *target;
    return obj;
}
