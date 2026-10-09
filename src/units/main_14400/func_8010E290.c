#include "common.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    void *vtable_08;
} Obj8010E290;

/* Initialized original vtable; its full type is unresolved. */
extern unsigned char D_8015D348[];

void *func_800AC0C0(Obj8010E290 *self, s32 a, s32 b);

/* Constructor: base init, install vtable, return the object. */
Obj8010E290 *func_8010E290(Obj8010E290 *obj, s32 kind) {
    func_800AC0C0(obj, 8, kind);
    obj->vtable_08 = D_8015D348;
    return obj;
}
