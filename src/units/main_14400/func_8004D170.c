#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Pos8004D170;

typedef struct {
    s32 x;
    s32 y;
    u8 field_8;
    u8 pad9[0x1E - 0x9];
    u8 state;
    u8 field_1F;
} Actor8004D170;

typedef struct {
    u8 pad0[0x1C];
    s32 field_1C;
    void *table;
    s32 field_24;
    s32 field_28;
    s32 field_2C;
    u8 pad30[0x5C - 0x30];
    s32 field_5C;
    s32 field_60;
    s32 field_64;
    s32 field_68;
} Task8004D170;

typedef struct {
    u8 pad0[0x3C];
    u16 field_3C;
} Unit8004D170;

extern u32 D_8013968C;
extern u32 D_8013960C;
extern u8 D_8013EA28[];
extern u8 D_8013EA4C[];
extern u8 D_8013EB2C[];
extern u8 D_8013EB58[];
extern u8 D_8013EB88[];
extern u8 D_8013ECEC[];
void func_8008B004(void *task);
void func_800887A4(void *task);
void func_800869E8(void *task);
void func_80086A28(void *task);
void func_8008A680(void *task);
void func_8008865C(void *task);
u8 func_800A8C00(Actor8004D170 *actor);
s32 func_80048B04(Pos8004D170 *pos);
Task8004D170 *func_800851B0(s32 id);
void func_80084A20(void);
void func_80084B80(void);
Unit8004D170 *func_8007946C(s32 side, s32 slot);
void func_80049414(Actor8004D170 *actor, s32 *x, s32 *y);
Task8004D170 *func_80085154(void (*handler)(void *), s32 value);
void func_8006D34C(void);
void func_800850F8(void (*handler)(void *), u16 value);
void func_80050E44(s32 id, Pos8004D170 *pos);
s32 func_80084AA8(void);
void func_80084A9C(s32 arg);

void func_8004D170(Actor8004D170 *actor, s32 arg) {
    Pos8004D170 pos;
    Pos8004D170 effectPos;
    Pos8004D170 *p;
    s32 slot;
    s32 sound;
    Task8004D170 *task;

    slot = func_800A8C00(actor);
    pos.x = actor->x;
    p = &pos;
    p->y = actor->y;
    if (!func_80048B04(p)) {
        return;
    }
    switch (D_8013968C) {
        case 0x20:
            if (!(D_8013960C & 1)) {
                break;
            }
            if (actor->state & 0xC) {
                if (arg > 0) {
                    func_800851B0(0x1A2);
                } else {
                    func_800851B0(0x1D);
                }
            } else if (arg > 0) {
                func_800851B0(0x1A3);
            } else {
                func_800851B0(0xB7);
            }
            break;
        case 0x7C:
        case 0x7D:
            func_800851B0(0xE);
            break;
        case 0x35:
            sound = 0x15;
            goto play_sound;
        case 0x36:
            sound = 0x19;
        play_sound:
            if (!(D_8013960C & 1)) {
                break;
            }
            func_80084A20();
            func_800851B0(sound)->field_1C = 4;
            func_80084B80();
            if ((actor->state >> 2) & 1) {
                Unit8004D170 *unit = func_8007946C(0, slot);
                s32 x;
                s32 y;

                func_80049414(actor, &x, &y);
                task = func_80085154(func_8008B004, slot);
                task->field_2C = 0;
                task->table = arg != 0 ? D_8013EA28 : D_8013EA4C;
                task->field_5C = x;
                task->field_60 = y;
                task->field_64 = unit->field_3C;
            }
            break;
        case 0x8F:
            task = func_80085154(func_800887A4, slot);
            task->field_24 = actor->field_1F;
            task->field_28 = arg;
            task->field_2C = actor->field_8;
            task->field_5C = actor->y;
            task->field_68 = actor->x;
            break;
        case 0x7A:
        case 0x7B:
            func_800851B0(arg > 0 ? 0x74 : 0x75);
            break;
        case 0x92: {
            Unit8004D170 *unit = func_8007946C(0, slot);
            s32 x;
            s32 y;
            s32 isBoss;
            u8 *table;

            func_8006D34C();
            func_80049414(actor, &x, &y);
            func_80085154(func_800869E8, 0);
            isBoss = actor->field_1F == 0x17;
            table = 0;
            switch (arg) {
                case 1:
                    if (isBoss) {
                        table = D_8013EB2C;
                    }
                    break;
                case 2:
                    if (isBoss) {
                        table = D_8013EB58;
                    }
                    break;
                case 3:
                default:
                    func_80085154(func_80086A28, 0);
                    table = D_8013ECEC;
                    if (isBoss) {
                        table = D_8013EB88;
                    }
                    break;
            }
            if (table == 0) {
                break;
            }
            task = func_80085154(func_8008B004, slot);
            task->field_2C = 0;
            task->table = table;
            task->field_5C = x;
            task->field_60 = y;
            task->field_64 = unit->field_3C;
            break;
        }
        case 0xA2:
            func_80085154(func_8008A680, slot)->field_24 = arg;
            break;
        case 0x76: {
            s32 msg;
            s32 saved;

            effectPos.x = actor->x;
            effectPos.y = actor->y;
            func_800850F8(func_8008865C, 7);
            func_80084A20();
            func_80050E44(0x142, &effectPos);
            func_80084B80();
            func_800850F8(func_8008865C, 0x14);
            msg = 0x147;
            if (arg != 0) {
                msg = 0x146;
            }
            func_80050E44(msg, &effectPos);
            saved = func_80084AA8();
            if (saved) {
                func_80084A9C(0);
            }
            func_80050E44(msg, &effectPos);
            func_80050E44(msg, &effectPos);
            func_80084A9C(saved);
            func_800850F8(func_8008865C, 0x28);
            break;
        }
    }
}
