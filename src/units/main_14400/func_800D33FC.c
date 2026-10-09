#include "common.h"
typedef unsigned char u8;
typedef struct { s32 first, second; } Pair;
typedef struct { u8 value; } Dir;
typedef struct { u8 pad_00[0xA]; u8 kind_0A; u8 pad_0B[0x75]; void *owner_80; } Unit;
extern Pair *D_801476B8;
extern s32 func_80049CB4(s32 id, ...);
extern void func_800498E4(s32 id, ...);
extern s32 func_800D2A64(void *obj);
extern void *func_800A2594(Pair *out, void *base, Dir direction);
extern void *func_800B4928(Pair *point);
extern s32 func_800E2044(Unit *unit);
extern char *func_800A3B20(Unit *unit);
static inline Dir *direction_set(Dir *direction, s32 value) {
    direction->value = value & 7;
    return direction;
}
static inline s32 direction_valid(s32 index) {
    return index < 8;
}
s32 func_800D33FC(void *obj) {
    Pair base, position;
    Dir direction;
    s32 i;
    Unit *unit;
    func_80049CB4(0x126, 0x1F);
    if ((func_800D2A64(obj) ^ 1) != 0) return 1;
    base.first = D_801476B8->first;
    base.second = D_801476B8->second;
    for (i = 0; direction_valid(i); i++) {
        func_800A2594(&position, &base, *direction_set(&direction, i));
        unit = func_800B4928(&position);
        if (unit && unit->kind_0A == 0x57 && unit->owner_80 == obj && (func_800E2044(unit) ^ 1) == 0) {
            func_800498E4(0x1CF, func_800A3B20(unit));
            return 1;
        }
    }
    return 1;
}
