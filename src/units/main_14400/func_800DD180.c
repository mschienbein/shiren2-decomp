#include "common.h"

/* Collection/item action link; func_800DA8A0 copies the source link to +8. */
typedef struct {
    void *collection;
    void *item;
} Link;

typedef struct {
    short id;
    short pad2;
    void *vtable;
    Link source;
    unsigned char field_10;
} Obj;

extern unsigned char D_80158778[];
Obj *func_800DA8A0(Obj *self, s32 kind, Link *source);

Obj *func_800DD180(Obj *obj, Link *source, unsigned char value)
{
    func_800DA8A0(obj, 0x1F, source);
    obj->vtable = D_80158778;
    obj->field_10 = value;
    return obj;
}
