#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef struct { s32 x; s32 y; } Pos;
typedef struct { Pos position; u8 pad8[0x16]; u8 field_1E; } Obj;
/* Task record returned by func_80085154 (D_801BA380 entry). */
typedef struct {
    u8 pad0[0x24];
    s32 field_24;
    s32 field_28;
    s32 field_2C;
    u8 pad30[0x2C];
    s32 field_5C;
    s32 field_60;
    s32 field_64;
    s32 field_68;
    s32 field_6C;
    s32 field_70;
} Task;
typedef struct { u8 pad0[0x84]; s32 field_84; } Player;
extern Player *D_801476B8;

extern void func_8008BB48(void *task);
extern void *func_80085154(void (*handler)(void *), s32 value);
extern s32 func_800E0F40(Obj *obj);
extern u16 func_800E08B0(void *obj);
extern u16 func_800E08F0(void *obj);
extern s32 func_80046240(void);
extern u32 func_800B1C6C(Pos *pos);
extern s32 func_800EB37C(Player *player);

static inline s32 is_eligible(Obj *source, Pos *p)
{
    if ((source->field_1E & 0xC) && !func_80046240() && !((D_80142F18.flags >> 2) & 1)
        && (func_800B1C6C(p) & 0x4000)) {
        return 1;
    }
    return 0;
}

void func_80048FD8(Obj *obj)
{
    Pos pos;
    Pos *p;
    Task *task = func_80085154(func_8008BB48, 0);
    Obj *source = obj;
    s32 stat;

    task->field_24 = D_80142F18.kind;
    task->field_28 = (u8)func_800E0F40(source);
    task->field_5C = (u16)func_800E08B0(source);
    task->field_60 = (u16)func_800E08F0(source);
    pos = source->position;
    task->field_64 = pos.y;
    task->field_70 = pos.x;
    p = &pos;
    task->field_2C = is_eligible(source, p);
    task->field_24 = D_80142F18.kind;
    task->field_68 = D_801476B8->field_84;
    if ((source->field_1E >> 2) & 1) {
        stat = (u8)func_800EB37C(D_801476B8);
    } else {
        stat = 100;
    }
    task->field_6C = stat;
}
