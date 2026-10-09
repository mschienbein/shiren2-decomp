#include "common.h"

/* List vtable slots (D_80154390): +0x3C func_800CE7A0 void *get(self, u32 index);
 * +0x44 func_800CE7D8 void set(self, s32 index, void *element). */
typedef struct { short delta, index; void *(*call)(void *, u32); } GetEntry;
typedef struct { short delta, index; void (*call)(void *, s32, void *); } SetEntry;
typedef struct { char pad0[0x38]; GetEntry get_38; SetEntry set_40; } Vtable;
typedef struct { void *owner_0; Vtable *vtbl_4; } Collection;

/* Item-family swap slot +0x5C: void (void *self, s32 a, s32 b), the type of the canonical
 * D_80154668 override func_800D09D8. Exchanges the elements at two indices; the indices are
 * converted to the u32 getter index (+0x3C) and passed unchanged to the s32 setter (+0x44). */
void func_800CE2B0(Collection *obj, s32 first, s32 second) {
    GetEntry *get = &obj->vtbl_4->get_38;
    void *saved = get->call((char *)obj + get->delta, (u32)first);
    void *other;
    SetEntry *set;
    get = &obj->vtbl_4->get_38;
    other = get->call((char *)obj + get->delta, (u32)second);
    set = &obj->vtbl_4->set_40;
    set->call((char *)obj + set->delta, first, other);
    set = &obj->vtbl_4->set_40;
    set->call((char *)obj + set->delta, second, saved);
}
