#include "common.h"
typedef short s16;
typedef unsigned char u8;
typedef unsigned short u16;
/* g++ 2.x vtable slots {this delta, index, function}: slot 12 (+0x64) takes only the receiver and
 * returns nothing; slot 19 (+0x9C) returns the inventory list. */
typedef struct { s16 delta; s16 index; void (*fn)(void *self); } VEntry12;
typedef struct { s16 delta; s16 index; void *(*fn)(void *self); } VEntry19;
typedef struct { char pad0[0x60]; VEntry12 slot12; char pad68[0x98 - 0x68]; VEntry19 inventory; } VTable;
typedef struct { char pad0[0x1E]; u8 flags; char pad1F[5]; VTable *vtbl; } Actor;
typedef struct { char pad0[4]; Actor *actor; char pad8[0x10]; s32 unk18; s32 unk1C; } Event;
typedef struct { char pad0; u8 kind; u8 flags2; char pad3[0xD - 3]; u8 flagsD; } Item;
extern u32 D_8013960C;
extern u16 D_8014767C;
s32 func_800CF140(void *list, u8 kind);
void func_800498E4(s32 message_id, ...);
s32 func_80049CB4(s32 id, ...);
s32 func_800ACEB4(Item *);
void func_800ACD34(Item *);
char *func_800AE674(void *obj);
s32 func_800AE498(Item *);
s32 func_800A533C(void *obj);
void func_800C94E8(void);
void func_800C95C8(Actor *);
#define VFN(obj, slot) ((obj)->vtbl->slot.fn)
#define VTHIS(obj, slot) ((char *)(obj) + (obj)->vtbl->slot.delta)
#define IS_PLAYER(a) (((a)->flags >> 2) & 1)
void func_80113C68(Item *item, Event *ev) {
    Actor *actor;
    s32 equip;
    s32 stacked;
    s32 force;
    s32 verbose;
    void *inv;
    s32 count;
    s32 doIt;
    s32 failed;
    s32 equipped;
    u8 kind;
    if (!(ev->actor->flags & 0xC)) {
        return;
    }
    actor = ev->actor;
    equip = ev->unk18 != 0;
    force = ev->unk1C != 0;
    stacked = force;
    verbose = D_8013960C & 1;
    equipped = item->flags2 & 4;
    if (equip == (equipped != 0)) {
        return;
    }
    inv = VFN(actor, inventory)(VTHIS(actor, inventory));
    if (inv == 0) {
        return;
    }
    count = func_800CF140(inv, 6);
    if (equip) {
        if (stacked && count >= 2) {
            func_800498E4(0x31);
            return;
        }
        item->flags2 |= 4;
        item->flagsD |= 2;
        doIt = IS_PLAYER(actor) && func_800ACEB4(item) == 1;
        if (doIt) {
            func_800ACD34(item);
        }
        if (verbose) {
            if (IS_PLAYER(actor)) {
                func_80049CB4(0x1134, 0);
            }
            func_80049CB4(0x1035, actor, count);
            if (IS_PLAYER(actor)) {
                func_80049CB4(6);
                func_80049CB4(0x12D);
                func_80049CB4(0x136);
                func_80049CB4(7);
            }
            func_800498E4(0x2B, func_800AE674(item));
            if (item->flagsD & 1) {
                func_80049CB4(0x12B);
                func_800498E4(0x2E, func_800AE674(item));
            }
        }
    } else {
        if (force) {
            failed = func_800AE498(item) ^ 1;
            if (failed) {
                return;
            }
        }
        item->flags2 &= ~4;
        if (verbose) {
            if (IS_PLAYER(actor)) {
                func_80049CB4(0x1134, 0);
            }
            func_80049CB4(0x1036, actor, count);
            if (IS_PLAYER(actor)) {
                func_80049CB4(6);
                func_80049CB4(0x137);
                func_80049CB4(0x136);
                func_80049CB4(7);
            }
            func_800498E4(0x2C, func_800AE674(item));
        }
    }
    VFN(actor, slot12)(VTHIS(actor, slot12));
    doIt = (D_8014767C & 1) && !(item->flagsD & 4);
    if (doIt) {
        func_80049CB4(0x132);
        func_800A533C(actor);
    }
    kind = item->kind;
    if ((D_8014767C & 1) && (kind == 0x7D || kind == 0x8A)) {
        func_800C94E8();
        if (kind == 0x7D) {
            func_800C95C8(actor);
        }
    }
}
