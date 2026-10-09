#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Position;
typedef struct { u8 pad0[0x5C]; s32 field_5C, field_60, field_64, field_68, field_6C, field_70; } Task;
extern s32 D_8013968C;
u8 func_800A8C00(void *actor);
void func_80088828(void *task);
void *func_80085154(void (*handler)(void *), s32 value);
void func_8004D090(void *actor, Position *from, Position *to, Position *last) {
    Position a, b, c;
    s32 value = func_800A8C00(actor);
    if (from && to) {
        Task *task;
        a.x = from->y;
        a.y = from->x;
        b.x = to->y;
        b.y = to->x;
        if (last) { c.x = last->y; c.y = last->x; }
        else { c.x = b.x; c.y = b.y; }
        if (D_8013968C == 0x12) {
            task = func_80085154(func_80088828, value);
            task->field_5C = a.x;
            task->field_68 = a.y;
            task->field_60 = b.x;
            task->field_6C = b.y;
            task->field_64 = c.x;
            task->field_70 = c.y;
        }
    }
}
