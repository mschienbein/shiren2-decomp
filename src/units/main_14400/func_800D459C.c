#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct Entry Entry;
typedef struct { s32 x, y; } Position;
typedef struct {
    u8 pad_00[8];
    s16 destroy_delta;
    s16 pad_0A;
    void (*destroy)(void *self, s32 flags);
} VTable;
typedef struct {
    u8 pad_00[0x1C];
    u16 field_1C;
    u8 pad_1E[6];
    const VTable *vtable;
} Actor;
typedef struct { u8 pad_00[2]; u8 field_02; } Item;
typedef struct {
    Entry *area;
    s32 kind;
    s32 field_08;
    s32 field_0C;
} State;
typedef struct Rng Rng;
extern Rng D_80147620;
/* Spawn-count parameter bytes, declared as the separate rodata objects the canonical tree uses for this table. */
extern const u8 D_8015691F, D_80156921, D_80156923, D_80156925, D_80156927;
extern s32 func_800ABDFC(void);
extern void func_800D3EF0(void *state);
extern s32 func_800C5844(void *rng, u8 base, u8 top);
extern Actor *func_800D4404(State *state);
extern Position *func_800A33DC(Position *position, Entry *area);
extern s32 func_800A4314(Actor *actor, Position *position);
extern u32 func_800B1C6C(Position *position);
extern void func_800A58FC(void *actor, Position *position);
extern void func_800E2124(Actor *actor);
extern s32 func_800C587C(void *rng, u8 chance);
extern void *func_800AAFC8(void);
extern s32 func_800AE2A4(Item *item, s32 allow_item, s32 allow_water, void *area);
extern void *func_800AAC20(s32 kind);

void func_800D459C(State *state, Entry *area, s32 allow_six, s32 extra) {
    s32 tries;
    s32 acceptable;
    s32 is_six;
    u8 actors, items;
    if (area == 0) {
        return;
    }
    tries = 9;
    do {
        state->kind = func_800ABDFC();
        if (allow_six || state->kind != 6) {
            break;
        }
    } while (--tries != -1);
    if (tries < 0) {
        return;
    }
    state->field_08 = 0;
    state->field_0C = 0;
    state->area = area;
    is_six = state->kind == 6;
    if (is_six && extra) {
        func_800D3EF0(state);
    }
    actors = func_800C5844(&D_80147620, D_80156921, D_8015691F);
    items = func_800C5844(&D_80147620, D_80156925, D_80156923);
    for (;;) {
        Position position;
        if (--actors == 255) {
            break;
        }
        tries = 10;
        for (;;) {
            Actor *actor;
            if (--tries == -1) {
                break;
            }
            actor = func_800D4404(state);
            if (actor == 0) {
                continue;
            }
            func_800A33DC(&position, state->area);
            acceptable = 0;
            if (func_800A4314(actor, &position)) {
                if (!(actor->field_1C & 8) || (func_800B1C6C(&position) & 0x2000)) {
                    acceptable = 1;
                }
            }
            if (acceptable) {
                func_800A58FC(actor, &position);
                func_800E2124(actor);
                break;
            }
            if (actor != 0) {
                actor->vtable->destroy((u8 *)actor + actor->vtable->destroy_delta, 3);
            }
        }
    }
    for (;;) {
        Item *item;
        if (--items == 255) {
            break;
        }
        tries = 10;
        for (;;) {
            Item *ordinary;
            if (--tries == -1) {
                break;
            }
            if (func_800C587C(&D_80147620, D_80156927)) {
                item = func_800AAFC8();
                if (item != 0 && func_800AE2A4(item, 1, 0, state->area)) {
                    item->field_02 |= 0x10;
                    break;
                }
            } else {
                ordinary = func_800AAC20(0);
                if (ordinary != 0 && func_800AE2A4(ordinary, 0, is_six, state->area)) {
                    break;
                }
            }
        }
    }
}
