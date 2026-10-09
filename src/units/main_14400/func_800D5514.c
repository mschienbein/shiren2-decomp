#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* Item method table at item+8: +0x8/+0xC destructor void (void *self, s32 flags). */
typedef struct { char pad[8]; s16 delta_8; s16 index_A; void (*destroy_C)(void *self, s32 flags); } ItemVTable;
typedef struct { u8 unk0; u8 kind1; char pad2[6]; ItemVTable *vtable_8; } Item;
/* Stack search holder: source pointer plus vtable (D_80149DB8 base, D_80149DA8 derived). */
typedef struct { void *source; const void *vtable; } Holder;

extern const u8 D_80149DB8[];
extern const u8 D_80149DA8[];
extern void *D_80147FD0[3];
extern void func_8013687C(Holder *holder);
extern Item *func_800D55F0(Holder *holder, u32 mode);
extern void *func_80128404(Item *item);

static inline void Holder_init(Holder *h) {
    h->vtable = D_80149DB8;
    func_8013687C(h);
    h->vtable = D_80149DA8;
}

static inline void Holder_fini(Holder *h) {
    h->vtable = D_80149DA8;
    h->source = 0;
    h->vtable = D_80149DB8;
}

void *func_800D5514(s32 index) {
    u8 slot = index;
    Holder holder;
    Item *item;
    void *result = 0;

    Holder_init(&holder);
    holder.source = D_80147FD0[slot];
    item = func_800D55F0(&holder, 2);
    if (item != 0) {
        if (item->kind1 == 0xF1) {
            result = func_80128404(item);
        }
        item->vtable_8->destroy_C((char *)item + item->vtable_8->delta_8, 3);
    }
    Holder_fini(&holder);
    return result;
}

