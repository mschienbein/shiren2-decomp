#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0x10]; short delta10, pad12; s32 (*locked14)(void *); } ItemVTable;
typedef struct { u8 field0, field1, flags, field3; s32 field4; ItemVTable *vtable; } Item;
typedef struct { u8 pad0[0x60]; short delta60, pad62; void (*refresh64)(void *); } UnitVTable;
typedef struct { u8 pad0[0x1E]; u8 flags; u8 pad1F[5]; UnitVTable *vtable; } Unit;
typedef struct { s32 kind; Unit *source; u8 pad8[0x10]; s32 enable, check; } Message;
extern u32 D_8013960C;
extern s32 func_8010BF6C(u8 *item, Unit *unit, s32 check);
extern s32 func_8010BEC4(Item *item, u8 kind);
extern void func_800EB35C(Unit *unit, u8 seconds);
extern s32 func_800EB37C(Unit *unit);
extern void func_800EB330(Unit *unit, u8 value);
extern void func_800ACD34(Item *item);
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800AE674(void *item);
extern void func_800498E4(s32 id, ...);
extern s32 func_800AE498(Item *item);
static __inline__ s32 enabled(Item *item, s32 mask) { return (item->flags & mask) != 0; }
static __inline__ s32 unit_active(Unit *unit) { return (unit->flags >> 2) & 1; }
void func_8010DA74(Item *item, Message *message) {
    s32 enable = message->enable != 0;
    s32 check = message->check != 0;
    Unit *unit = message->source;
    s32 visible = D_8013960C & 1;
    if (enable == enabled(item, 4)) return;
    if (enable) {
        s32 failed = func_8010BF6C((u8 *)item, unit, check) != 1;
        s32 special = 0;
        if (failed) return;
        if ((u8)func_8010BEC4(item, 0x6E)) special = unit_active(unit);
        if (special) {
            func_800EB35C(unit, 1);
            if ((u8)func_800EB37C(unit) >= 2) func_800EB330(unit, 1);
        }
        item->flags |= 4;
        if (unit_active(unit)) func_800ACD34(item);
        if (visible) {
            func_80049CB4(0x1129, 3);
            func_80049CB4(0x33, unit, item);
            func_800498E4(0x2B, func_800AE674(item));
            if (item->vtable->locked14((char *)item + item->vtable->delta10)) {
                func_80049CB4(0x12B);
                func_800498E4(0x2E, func_800AE674(item));
            }
        } else func_80049CB4(0x84, unit, item);
    } else {
        if (check) {
            s32 failed = func_800AE498(item) != 1;
            if (failed) return;
        }
        item->flags &= ~4;
        if (visible) {
            func_80049CB4(0x1129, 3);
            func_80049CB4(0x34, unit, item);
            func_800498E4(0x2C, func_800AE674(item));
        } else func_80049CB4(0x85, unit, item);
    }
    if (unit->flags & 0xC) unit->vtable->refresh64((char *)unit + unit->vtable->delta60);
}
