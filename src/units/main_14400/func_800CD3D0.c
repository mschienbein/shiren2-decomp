#include "common.h"

typedef short s16;

/* Vtable slots: s16 this-adjustment, s16 unused, then the target. Prototypes follow the
 * targets bound in the list tables D_80154390/D_80154438/D_80154550 (D_80154390 is
 * installed by func_800CE6A0): +0x24 func_800CE710 (element count, read with lbu and
 * consumed at full int width here and by func_800CD468's signed count - 1 loop, so the
 * count is an int result), +0x3C func_800CE7A0 (void *get(self, u32)), +0x4C
 * func_800CE81C/func_800CEE58 (void remove(self, s32)); item slot +0xC is the
 * destructor (self, s32 flags), e.g. func_80117050 in D_8015DA98. */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*func)(void *self);
} CountSlot800CD3D0;

typedef struct {
    s16 delta;
    s16 index;
    void *(*func)(void *self, u32 index);
} GetSlot800CD3D0;

typedef struct {
    s16 delta;
    s16 index;
    void (*func)(void *self, s32 index);
} RemoveSlot800CD3D0;

typedef struct {
    s16 delta;
    s16 index;
    void (*func)(void *self, s32 flags);
} DestroySlot800CD3D0;

typedef struct {
    char pad0[0x8];
    DestroySlot800CD3D0 destroy;
} ItemVtable;

typedef struct {
    char pad0[0x8];
    ItemVtable *vtable;
} Item800CD3D0;

typedef struct {
    char pad0[0x20];
    CountSlot800CD3D0 count;
    char pad28[0x38 - 0x28];
    GetSlot800CD3D0 get;
    char pad40[0x48 - 0x40];
    RemoveSlot800CD3D0 remove;
} ListVtable;

typedef struct {
    void *pool;
    ListVtable *vtable;
} List800CD3D0;

void func_800CD3D0(List800CD3D0 *list, u32 index) {
    Item800CD3D0 *item;

    if (index < list->vtable->count.func((char *)list + list->vtable->count.delta)) {
        item = list->vtable->get.func((char *)list + list->vtable->get.delta, index);
        list->vtable->remove.func((char *)list + list->vtable->remove.delta, index);
        if (item != 0) {
            item->vtable->destroy.func((char *)item + item->vtable->destroy.delta, 3);
        }
    }
}
