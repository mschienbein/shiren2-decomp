#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 first, second; } Pair;
typedef struct { s32 x, y, z; } Vec3i;
typedef struct Actor Actor;
struct Actor {
    void (*callback_00)(Actor *); u8 pad_04[0xE]; u16 flags_12;
    u8 pad_14[0x20]; float value_34; u8 pad_38[0x24];
    s32 from_x_5C, to_x_60, field_64, from_y_68, to_y_6C;
};
extern void *func_800A27A4(void *direction, void *from, void *to);
extern void *func_80085938(s32 id, s32 track, Vec3i position, s32 arg4, s32 arg5, s32 flags, s32 arg7);
extern s32 func_800A99D0(void);
extern void func_800865A0(Actor *actor);

static inline Pair *copy_cell(Pair *out, Pair *cell) {
    out->first = cell->first;
    out->second = cell->second;
    return out;
}
static inline void cell_position(Vec3i *out, Pair *cell) {
    out->x = (cell->second << 5) + 16;
    out->y = (cell->first << 5) + 16;
    out->z = 0;
}
static inline s32 hidden_mode(void) {
    return (D_80142F18.flags >> 2) & 1;
}
void func_80050CEC(s32 id, s32 arg, Pair *from, Pair *to, float value) {
    Pair target;
    Vec3i position;
    u8 direction;
    Actor *actor;
    s32 visible;
    s32 facing;
    func_800A27A4(&direction, from, copy_cell(&target, to));
    facing = direction;
    cell_position(&position, from);
    actor = func_80085938(id, -1, position, arg, facing, 0, 0);
    actor->flags_12 |= 4;
    visible = 0;
    if (func_800A99D0()) visible = hidden_mode() == 0;
    if (!visible) actor->flags_12 |= 0x100;
    if ((u32)(id - 0x1BB) >= 2) actor->flags_12 |= 0x1000;
    actor->from_x_5C = from->second;
    actor->from_y_68 = from->first;
    actor->to_x_60 = to->second;
    actor->to_y_6C = to->first;
    actor->callback_00 = func_800865A0;
    actor->value_34 = value;
}
