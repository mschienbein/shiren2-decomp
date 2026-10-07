#include "common.h"
/* Item vtable: slot +0x40/+0x44 (entry 8) takes (self, unit); its targets
 * (func_80117770, func_80118830, ...) dereference unit as an actor. */
typedef struct { char pad[0x40]; short offset; void (*fn)(void *self, void *unit); } VT;
typedef struct { s32 x0; s32 x4; VT *vt; } Obj;
/* Entry 9 (+0x48/+0x4C): user is unused here; overrides pass it on as a pointer
 * (func_80111D80 -> func_80111E08, func_801173C8 -> func_80136910). */
void func_80117000(Obj *p, void *user, void *unit) {
    VT *vt = p->vt;
    vt->fn((char *)p + vt->offset, unit);
}
