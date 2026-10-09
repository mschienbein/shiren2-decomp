#include "common.h"
/* 0x34-byte overlay secondary base at +0x78, constructed by func_801F2AC0 in
 * func_800EF930 (vtable word at +0x30 = obj+0xA8); +0x18 holds its kind halfword. */
typedef struct { unsigned char pad0[0x18]; unsigned short kind18; unsigned char pad1A[0x16]; void *vtable_30; } Subobject;
typedef struct { unsigned char pad0[0x78]; Subobject sub78; } Object;
extern void func_801F216C(unsigned short kind, void *object);
extern s32 func_801F2BE8(Subobject *subobject, s32 mode);
extern void *func_800A6538(void *direction, void *object, void *target);
extern void func_800A665C(void *object, unsigned char *direction);
void func_800EFAB4(Object *object, void *target) {
    Subobject *sub = object ? &object->sub78 : 0;
    unsigned char direction;
    func_801F216C(sub->kind18, object);
    if (func_801F2BE8(object ? &object->sub78 : 0, 1)) {
        unsigned char *cursor = &direction;
        func_800A6538(cursor, object, target);
        func_800A665C(object, cursor);
    }
}
