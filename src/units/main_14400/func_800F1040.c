#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    char pad0[0x1C];
    u16 flags;
    u8 field_1E;
} Unit;

typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
extern s32 func_800A455C(void *ctx, Unit *unit, s32 arg);
extern u32 func_800B1C6C(Unit *unit);
extern s32 func_80049CB4(s32 id, ...);

static inline s32 isDemoMode(void) {
    return D_80142F18.mode == 0x4F;
}

s32 func_800F1040(void *ctx, Unit *unit, s32 id) {
    s32 blocked = 0;

    if (unit == 0 || !func_800A455C(ctx, unit, 1) || (func_800B1C6C(unit) & 0x4000) || (unit->flags & 1) ||
        isDemoMode()) {
        blocked = 1;
    }
    if (blocked) {
        func_80049CB4(id, ctx);
        return 2;
    }
    return (unit->field_1E & 3) != 0;
}
