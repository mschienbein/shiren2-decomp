#include "common.h"

/* Widget-derived object: base constructor func_800953C0 (returns the object), then
 * its own method table at +0x4C. */
typedef struct {
    char pad0[0x4C];
    void *field_4C;
} Obj;

extern char D_80152B78[];
extern Obj *func_800953C0(Obj *obj);

Obj *func_8009D8D4(Obj *obj)
{
    func_800953C0(obj);
    obj->field_4C = D_80152B78;
    return obj;
}
