#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
typedef short s16;

/* g++ 2.x vtable slot: this-adjustment delta, index, function pointer. */
typedef struct {
    s16 delta;
    s16 index;
    void *(*fn)(void *self);
} VtblEntry;


typedef struct {
    u8 pad0[0x1E];
    u8 flags_1E;
    u8 pad1F[0x5];
    VtblEntry *vtbl_24;
} Other_8010EDD4;

extern s32 func_80049CB4(s32 id, ...);
extern void func_800498E4(s32 message_id, ...);
extern void func_800D3650(Item *obj);
extern void func_800CD364(void *target, Item *obj);

s32 func_8010EDD4(Item *obj, Other_8010EDD4 *other) {
    VtblEntry *entry;
    void *target;

    /* Kind-3 D_8015D518 dispatch: 80110258 -> 801106BC -> 80110748 -> F7B4/F8A0. */
    if (--obj->payload.seals.field20 == 0) {
        func_800498E4(0xFF, func_800AE674(obj));
        func_80049CB4(0x128, 0x17);
        if ((other->flags_1E >> 2) & 1) {
            func_800D3650(obj);
        }
        entry = &other->vtbl_24[19];
        target = entry->fn((u8 *)other + entry->delta);
        if (target != 0) {
            func_800CD364(target, obj);
        }
        return 1;
    }
    return 0;
}
