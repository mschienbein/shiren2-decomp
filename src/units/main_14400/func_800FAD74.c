#include "common.h"
typedef struct { s32 field_0; s32 field_4; } Pos800FAD74;
typedef struct { short offset; short pad; s32 (*func)(void *self, void *other); } VtEntry800FAD74;
typedef struct { unsigned char pad0[0xB0]; VtEntry800FAD74 entry_B0; } Vtable800FAD74;
typedef struct { Pos800FAD74 pos; unsigned char pad8[0x1C]; Vtable800FAD74 *vtable; } Obj800FAD74;
typedef struct { unsigned char pad0[0x1E]; unsigned char flags; } Item800FAD74;
Item800FAD74 *func_800A6CF0(Obj800FAD74 *obj);
s32 func_800F0EC4(Obj800FAD74 *obj);
s32 func_800F3358(Obj800FAD74 *obj);
s32 func_800E20CC(void *obj);
s32 func_80049CB4(s32 id, ...);
char *func_800A3B20(void *obj);
void func_800497F0(s32 id, ...);
s32 func_800FAD74(Obj800FAD74 *obj) {
    Item800FAD74 *item = func_800A6CF0(obj);
    s32 blocked = 0;
    Pos800FAD74 pos;
    s32 a;
    if (func_800F0EC4(obj) != 0 || (item != 0 && ((item->flags >> 1) & 1))) blocked = 1;
    if (blocked) return func_800F3358(obj);
    if (func_800E20CC(obj) != 0) {
        return obj->vtable->entry_B0.func((char *)obj + obj->vtable->entry_B0.offset, item);
    }
    {
        Pos800FAD74 *p = &pos;
        p->field_0 = obj->pos.field_0;
        p->field_4 = obj->pos.field_4;
        a = func_80049CB4(0xDA, p);
    }
    func_800497F0(0x22A, a, func_800A3B20(obj));
    return 1;
}
