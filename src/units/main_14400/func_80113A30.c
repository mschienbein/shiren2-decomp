#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 pad_00[8];
    signed short delta_08;
    unsigned short reserved_0A;
    void (*destroy_0C)(void *, s32);
} ItemVtable;
typedef struct { u8 pad_00[8]; ItemVtable *vtable_08; } Item;
typedef struct {
    u8 pad_00[0x98];
    signed short delta_98;
    unsigned short reserved_9A;
    void *(*list_9C)(void *);
} OwnerVtable;
typedef struct {
    u8 pad_00[0x1E];
    u8 flags_1E;
    u8 pad_1F[5];
    OwnerVtable *vtable_24;
} Owner;
extern u32 D_8013960C;
extern char *func_800AE674(void *item);
extern char *func_800A3B20(Owner *owner);
extern void func_800498E4(s32 id, ...);
extern void func_80049BF0(s32 mode);
extern s32 func_800CD2BC(void *list, void *item);
extern void func_800D3650(void *item);

void func_80113A30(Item *item, Owner *owner)
{
    u8 enabled;
    D_8013960C = (D_8013960C << 1) | 1;
    enabled = (owner->flags_1E >> 2) & 1;
    if (enabled) {
        func_800498E4(0x104, func_800AE674(item));
    } else {
        char *owner_name = func_800A3B20(owner);
        func_800498E4(0x105, owner_name, func_800AE674(item));
    }
    func_80049BF0(0);
    D_8013960C >>= 1;
    if (owner != 0) {
        OwnerVtable *vtable = owner->vtable_24;
        void *list = vtable->list_9C((char *)owner + vtable->delta_98);
        if (list != 0) {
            func_800CD2BC(list, item);
        }
    }
    func_800D3650(item);
    if (item != 0) {
        ItemVtable *vtable = item->vtable_08;
        vtable->destroy_0C((char *)item + vtable->delta_08, 3);
    }
}
