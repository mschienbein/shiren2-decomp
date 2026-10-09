#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Position;
typedef struct { u8 pad0[8]; short adjust_8; short padA; void (*destroy_C)(void *, s32); } ItemVTable;
typedef struct { u8 pad0[0x68]; short adjust_68; short pad6A; u32 (*get_6C)(void *); u8 pad70[0x20]; short adjust_90; short pad92; s32 (*call_94)(void *, s32, s32, u8, s32); } ActorVTable;
typedef struct { u8 pad0[0x1E]; u8 flags_1E; u8 pad1F[5]; ActorVTable *field_24; u8 pad28[0x54]; u16 flags_7C; } Actor;
typedef struct { u8 kind, id, flags, mode; u8 pad4[4]; ItemVTable *field_8; u8 flags_C; } Item;
u32 func_800B1C6C(Position *position);
s32 func_800B56F0(void *position);
s32 func_800B5650(void *position);
s32 func_80049CB4(s32 id, ...);
void *func_800B4928(Position *position);
s32 func_800A58B8(Actor *self);
char *func_800A3B20(void *self);
void func_800497F0(s32 id, ...);
void func_80049C90(s32 mode, s32 message);
void func_800E0CF4(Actor *self, u16 percent);
void func_800E20F0(Actor *self);
void *func_800B4D80(Position *position);
char *func_800AE674(void *self);
void func_80112E7C(Item *self);
void *func_800B31E8(void *position, s32 team);
void func_800AD868(Position *position);
void func_8010E2CC(Item *self);
static inline s32 guarded(Actor *actor) { return (actor->flags_1E >> 4) & 1; }
static inline s32 shrinks(u16 flags) { return (flags >> 11) & 1; }
static inline s32 transforms(u16 flags) { return (flags >> 12) & 1; }
static inline u8 kindOf(Item *item) { return item->kind; }
static inline s32 sealed(Item *item) { return (item->flags_C >> 1) & 1; }
static inline s32 changedId(Item *item) { return item->id ^ 0xA4; }
/* func_801280F0 passes its receiver in a0; this effect only reads the position. */
void func_80127DA0(void *self, Position *position) {
    Actor *actor;
    Item *item;
    s32 message;
    u8 kind;
    char *name;
    if (func_800B1C6C(position) & 0x4000) return;
    if (func_800B56F0(position)) {
        func_800B5650(position);
        func_80049CB4(0x11B, position);
        return;
    }
    actor = func_800B4928(position);
    if (actor && func_800A58B8(actor) != 1) {
        s32 actorMessage = func_80049CB4(0x11B, position);
        func_800497F0(0xBE, actorMessage, func_800A3B20(actor));
        func_80049C90(1, actorMessage);
        if (guarded(actor)) {
            u16 flags = actor->flags_7C;
            if (shrinks(flags)) {
                ActorVTable *table = actor->field_24;
                if ((u16)table->get_6C((u8 *)actor + table->adjust_68) >= 2) {
                    func_800E0CF4(actor, 50);
                    func_800497F0(0x12A, actorMessage, func_800A3B20(actor));
                    return;
                }
                func_800497F0(0x222, actorMessage);
                return;
            }
            if (transforms(flags)) {
                ActorVTable *table = actor->field_24;
                table->call_94((u8 *)actor + table->adjust_90, 0, 2, 254, 0);
                return;
            }
        }
        if (actor->flags_1E & 0x7C) func_800E20F0(actor);
        func_800497F0(0x222, actorMessage);
        return;
    }
    message = func_80049CB4(0x10B, position);
    item = func_800B4D80(position);
    if (item) {
        kind = kindOf(item);
        if (item->mode != 1 && kind != 15 && kind != 19) {
            name = func_800AE674(item);
            func_800497F0(0xBE, message, name);
            if (kind == 2) {
                func_80049C90(1, message);
                func_800497F0(0xE4, message, name);
                func_80112E7C(item);
                return;
            } else if (kind == 16) {
                if (item->flags & 0x10) {
                    item->flags &= 0xEF;
                    func_80049CB4(0xD7, position);
                }
                func_80049C90(1, message);
                {
                    s32 blocked = 0;
                    if (sealed(item) || func_800B31E8(position, 0x15)) blocked = 1;
                    if (blocked) { func_800497F0(0x224, message); return; }
                }
                func_800497F0(0xF5, message, name);
                func_80049CB4(0x107, position);
                func_800AD868(position);
                if (item) {
                    ItemVTable *table = item->field_8;
                    table->destroy_C((u8 *)item + table->adjust_8, 3);
                }
                return;
            } else if (kind == 8) {
                if (changedId(item)) {
                    func_80049C90(1, message);
                    func_8010E2CC(item);
                    func_800497F0(0x229, message, name, func_800AE674(item));
                }
            }
        }
    }
}
