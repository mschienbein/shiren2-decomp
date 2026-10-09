#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { s32 x, y; } Position;
typedef struct Unit Unit;
typedef struct { s16 delta; s16 pad_02; s32 (*call)(void *, s32, s32, u8, s32); } ChangeSlot;
typedef struct {
    u8 pad_00[0x68]; s16 delta_68; s16 pad_6A; u32 (*amount)(void *);
    u8 pad_70[0x20]; ChangeSlot change;
} UnitTable;
struct Unit { Position position; u8 pad_08[0x16]; u8 field_1E; u8 pad_1F[5]; UnitTable *field_24; };
typedef struct { u8 pad_00[0x40]; s16 delta_40; s16 pad_42; void (*apply)(void *, Unit *); } ActionTable;
typedef struct { u8 pad_00[8]; ActionTable *field_08; } Action;
extern u16 D_801569F0;
extern s32 func_800E1CC4(Unit *, s32);
extern void func_800E0CF4(Unit *, u16);
extern s32 func_80049CB4(s32, ...);
extern char *func_800A3B20(Unit *);
extern void func_800497F0(s32, ...);
extern s32 func_800A7BE4(Unit *, void *);

static __inline__ Position *copy_position(Position *out, Position *source) {
    out->x = source->x;
    out->y = source->y;
    return out;
}

static __inline__ s32 effect_scale(Unit *unit) {
    return func_800E1CC4(unit, 3) ? 2 : 1;
}

static __inline__ void reduce_amount(Unit *unit, s32 count) {
    s32 i = 0;
    if ((u8)count) {
        s32 limit = (u8)count;
        s32 percent = 100 - D_801569F0;
        do {
            func_800E0CF4(unit, percent);
            i++;
        } while (i < limit);
    }
}

static __inline__ void change_amount(Unit *unit, s32 scale) {
    ChangeSlot *slot = &unit->field_24->change;
    void *self = (u8 *)unit + slot->delta;
    s32 amount = -scale;
    s32 event = 0x11;
    if (amount > 0) {
        event = 0x12;
    }
    slot->call(self, 0, event, 0xFE, amount);
}

/* Item vtable slot +0x4C pair action (self, source, target): the dispatcher supplies the source
 * unit pointer, which is forwarded to func_800A7BE4 as its message source. */
void func_80118A2C(Action *action, void *source, Unit *unit) {
    if (!(unit->field_1E & 0xC)) {
        s32 scale = effect_scale(unit);
        if ((u16)unit->field_24->amount((u8 *)unit + unit->field_24->delta_68) >= 2) {
            reduce_amount(unit, scale);
            {
                Position position;
                s32 event = func_80049CB4(0xDA, copy_position(&position, &unit->position));
                func_800497F0(0x12A, event, func_800A3B20(unit));
            }
        }
        change_amount(unit, scale);
        func_800A7BE4(unit, source);
    } else {
        action->field_08->apply((u8 *)action + action->field_08->delta_40, unit);
    }
}
