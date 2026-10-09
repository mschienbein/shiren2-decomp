#include "common.h"
typedef unsigned char u8;
/* Item-set member (family D_80154300, vptr at member+4): slot +0x1C is void (void *self, s32 value). */
typedef struct { u8 pad0[0x18]; short delta18, pad1A; void (*set1C)(void *self, s32 value); } MemberVTable;
typedef struct { void *owner; MemberVTable *vtable; } Member;
typedef struct { u8 pad0[0x10]; short delta10, pad12; s32 (*locked14)(void *); } ItemVTable;
typedef struct { u8 type, field1, flags, field3, count; u8 pad5[3]; ItemVTable *vtable; Member member; } Item;
typedef struct { u8 pad0[0x98]; short delta98, pad9A; void *(*inventory9C)(void *); } UnitVTable;
typedef struct { u8 pad0[0x1E]; u8 flags; u8 pad1F[5]; UnitVTable *vtable; } Unit;
extern u32 D_8013960C;
extern s32 func_800A99D0(void);
extern void func_800ACDD8(void *item);
extern s32 func_800CD2BC(void *container, void *item);
extern void *func_8011422C(u8 *item);
extern s32 func_800CD5C0(void *container, Item *item);
extern s32 func_800CD278(Member *member);
extern void *func_800CF058(void *container, u8 kind);
extern s32 func_8010C38C(Item *item);
extern void func_800498E4(s32 id, ...);
extern s32 func_800A08D8(s32 mode, s32 key, s32 selection);
extern void func_800AE518(void *item, void *unit, s32 enable, s32 check);
extern s32 func_800CD090(void *container, void *item);
extern s32 func_800CD538(void *container, Item *item);
extern void func_800AD868(Unit *unit);
extern s32 func_80049CB4(s32 id, ...);
extern void func_80049BF0(s32 mode);
extern void *func_80127C28(void);
extern s32 func_800AC670(void *item);
extern s32 func_800CD1FC(void *container);
static __inline__ s32 active(Unit *unit) { return (unit->flags >> 2) & 1; }
static __inline__ void set_count(Member *member, s32 count) {
    member->vtable->set1C((char *)member + member->vtable->delta18, count);
}
Item *func_80121048(Item *item, Unit *unit, Item *element) {
    s32 missing;
    void *container;
    Item *selected;
    s32 blocked = 0;
    if (!active(unit) || func_800A99D0()) blocked = 1;
    if (blocked) return element;
    container = unit->vtable->inventory9C((char *)unit + unit->vtable->delta98);
    func_800ACDD8(item);
    func_800CD2BC(container, element);
    func_800CD5C0(func_8011422C((u8 *)item), element);
    if (func_800CD278(&item->member) <= 0) {
        func_800498E4(0xB2);
        return 0;
    }
    selected = func_800CF058(container, 3);
    if (selected == 0) {
        selected = func_800CF058(container, 4);
        if (selected != 0) {
            s32 failed = func_8010C38C(selected) != 1;
            if (failed) selected = 0;
        }
    }
    if (selected != 0) {
        if (selected->vtable->locked14((char *)selected + selected->vtable->delta10)) {
            func_800498E4(selected->type == 3 ? 0xB3 : 0xB4);
            func_800A08D8(1, -1, 0);
            return 0;
        }
        D_8013960C <<= 1;
        func_800AE518(selected, unit, 0, 0);
        D_8013960C >>= 1;
    }
    if (func_800CD090(container, item) < 0) {
        func_800CD538(container, item);
        func_800AD868(unit);
    }
    item->flags |= 4;
    func_80049CB4(0x84, unit, item);
    func_800498E4(0xB1);
    func_80049BF0(1);
    selected = func_80127C28();
    missing = func_800AC670(selected) != 1;
    if (missing) {
        set_count(&item->member, func_800CD1FC(func_8011422C((u8 *)item)) + selected->count);
    } else {
        set_count(&item->member, func_800CD1FC(func_8011422C((u8 *)item)));
        return 0;
    }
    return selected;
}
