#include "common.h"

/* Embedded 0x10-byte list: pool, vtable, buffer, and byte-sized counters. */
typedef struct {
    void *pool;
    void *vtable;
    unsigned char *buffer;
    unsigned char capacity, limit, count;
    unsigned char pad0F;
} ItemList;

typedef struct {
    char pad0[0xA8];
    ItemList member_A8;
} Obj80101B24;

extern void func_800CE6A0(ItemList *obj, s32 flags);
extern void func_800EFD28(Obj80101B24 *self, s32 flags);
extern void func_800A3918(Obj80101B24 *obj);

/* Destructor: destroy the member at +0xA8, then the base, free storage when bit 0 is set. */
void func_80101B24(Obj80101B24 *self, s32 flags) {
    func_800CE6A0(&self->member_A8, 2);
    func_800EFD28(self, 0);
    if (flags & 1) {
        func_800A3918(self);
    }
}
