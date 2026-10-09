#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef struct { s32 x, y; } Point;
typedef struct {
    Point position;
    u8 field_08, pad_09, field_0A;
    u8 pad_0B[0x13];
    u8 field_1E, field_1F;
    u8 pad_20[0x55];
    u8 field_75;
} Actor;
typedef struct { u8 kind, id, pad_02[10], flags; } Item;
typedef struct {
    u8 pad_00[0x1C]; s32 field_1C;
    void *script_20;
    s32 field_24, field_28, field_2C;
    u8 pad_30[0x2C];
    u32 field_5C; s32 field_60, field_64;
} Task;
typedef struct {
    u8 pad_00[0xC]; s16 field_0C, field_0E;
    u8 pad_10[0x24]; s8 field_34;
    u8 pad_35[7]; u16 field_3C;
} Unit;
/* Equipment descriptor (4 bytes): +0 model/tile id (lhu), +2 percentage
 * byte (lbu in func_80077C4C), +3 never accessed. */
typedef struct { u16 id; u8 percent; u8 unk3; } EquipInfo;
extern s32 D_8013968C;
extern u8 D_80139618[];
extern u8 D_8013E9F8[], D_8013EA70[], D_8013EAA8[];
extern u8 D_8013EEE4[], D_8013EF00[], D_8013EF20[];
extern u8 D_8013F0D4[], D_8013F0F4[], D_8013F114[];
extern EquipInfo D_8013F24C[], D_8013F2EC[];
extern u8 func_800A8C00(void *actor);
extern Task *func_80085154(void (*handler)(void *), s32 value);
extern Task *func_800851B0(s32 id);
extern void func_800850F8(void (*handler)(void *), u16 value);
extern void func_8008B004(void *task);
extern void func_8008865C(void *task);
extern void func_800869E8(void *task);
extern void func_8008B678(void *task);
extern void func_8008872C(void *task);
extern void func_8008876C(void *task);
extern s32 func_8007920C(s32 kind, s32 id, s32 x, s32 y);
extern void func_8004977C(s32 index, s32 value);
extern Unit *func_8007946C(s32 side, s32 slot);
extern void func_80084A68(void);
extern void func_80084A20(void);
extern void func_80084B80(void);
extern void func_80049414(Actor *actor, u32 *flags, s32 *mode);
extern void *func_80050FA0(s32 id, Actor *actor, s32 mode, s32 flags);
extern void func_80051264(s32 id, Actor *actor, s32 flags);
extern Item *func_800E8A68(Actor *actor, u8 slot);
extern void func_80050E44(s32 id, Point *position);
extern void *func_800510E0(s32 id, Actor *actor, s32 mode, s32 flags, s32 tile);
extern s32 func_8010EBA4(void *item);

static inline Point *copy_position(Point *out, const Point *in)
{
    out->x = in->x;
    out->y = in->y;
    return out;
}

void func_8004E440(Actor *actor, Item *item)
{
    Point position;
    u32 flags1;
    s32 mode1;
    u32 flags2;
    s32 mode2;
    s32 index = func_800A8C00(actor);
    Task *task;
    s32 tile;

    switch (D_8013968C) {
    case 0x16: {
        s32 offset = 0;
        s32 kind = actor->field_1F;
        Unit *unit;
        s32 direction;
        s32 pair;
        s32 original = index;
        task = func_80085154(func_8008B004, index);
        task->field_2C = 4;
        if (kind != 0x1A) {
            if (kind == 0x1B) task->script_20 = D_8013F0F4;
        } else {
            task->script_20 = D_8013F0D4;
            offset = 0x10;
        }
        index = func_8007920C(item->kind, item->id, 10, 10);
        func_8004977C(index, original);
        unit = func_8007946C(4, index);
        direction = actor->field_08;
        unit->field_34 = (u32)(direction - 1) < 4 ? -1 : 1;
        pair = offset + direction * 2;
        unit->field_0C = (s8)D_80139618[pair];
        unit->field_0E = (s8)D_80139618[pair + 1];
        task = func_80085154(func_8008B004, index);
        task->field_2C = 4;
        task->script_20 = D_8013F114;
        break;
    }
    case 0x30: {
        Unit *unit;
        s32 special;
        s32 original = index;
        func_80084A68();
        func_80049414(actor, &flags1, &mode1);
        func_80084A20();
        task = func_80085154(func_8008B004, index);
        func_80084B80();
        task->field_2C = 0;
        unit = func_8007946C(0, index);
        task->field_5C = flags1;
        task->field_60 = mode1;
        task->field_64 = unit->field_3C;
        special = 0;
        if ((item->flags & 1) || item->id == 0x20) special = 1;
        if (special) task->script_20 = D_8013EF00;
        else task->script_20 = D_8013EF20;
        index = func_8007920C(0, 0x13D, 10, 10);
        func_8004977C(index, original);
        task = func_80085154(func_8008B004, index);
        task->field_2C = 4;
        task->script_20 = D_8013EEE4;
        func_80084A20();
        func_800851B0(0xF)->field_1C = 1;
        func_80084B80();
        switch (item->id) {
        case 24: case 25: case 26: case 31: case 33:
        case 40: case 41: case 42: case 43: case 44:
        case 47: case 48: case 49:
            if ((actor->field_1E >> 2) & 1) {
                func_80084A20();
                func_800850F8(func_8008865C, 0xB);
                func_80085154(func_800869E8, 0);
                func_80084B80();
            }
            /* These item types share the final sound. */
        case 29: case 34:
            func_800850F8(func_8008865C, 4);
            break;
        case 27: case 28: case 30: case 35:
            break;
        default:
            func_800850F8(func_8008865C, 0x18);
            break;
        }
        break;
    }
    case 0x26:
        if (actor->field_1F == 0x54) {
            func_80084A68();
            func_800850F8(func_8008B678, 0);
            func_80084A20();
            func_80050FA0(0x113, actor, actor->field_08, 0);
            func_80084B80();
            func_80084A20();
            func_80051264(0x121, actor, 0);
            func_80084B80();
        }
        break;
    case 0x2A:
        tile = D_8013F24C[func_800E8A68(actor, 3)->id - 0x32].id;
        func_800850F8(func_8008865C, 10);
        func_80051264(0x13F, actor, 0x1000C);
        func_800510E0(0x15E, actor, 4, 0x10000, tile);
        break;
    case 0x2B:
        tile = D_8013F2EC[func_800E8A68(actor, 4)->id - 0x59].id;
        func_800850F8(func_8008865C, 10);
        func_80051264(0x13E, actor, 0x1000C);
        func_800510E0(0x15D, actor, 2, 0x10000, tile);
        break;
    case 0x2C:
        copy_position(&position, &actor->position);
        func_800850F8(func_8008865C, 7);
        func_80084A20();
        func_80050E44(0x142, &position);
        func_80084B80();
        func_800850F8(func_8008865C, 0x18);
        switch (item->kind) {
        case 3:
            tile = D_8013F24C[item->id - 0x32].id;
            func_80084A20();
            func_80051264(0x143, actor, 0x90000);
            func_80084B80();
            func_800510E0(0x159, actor, 4, 0x10000, tile);
            break;
        case 4:
            tile = D_8013F2EC[item->id - 0x59].id;
            func_80084A20();
            func_80051264(0x143, actor, 0x110000);
            func_80084B80();
            func_800510E0(0x158, actor, 2, 0x10000, tile);
            break;
        case 6:
            func_80084A20();
            func_80051264(0x143, actor, 0x90000);
            func_80084B80();
            func_800510E0(0x159, actor, 2, 0x10000, 0x189);
            break;
        }
        break;
    case 0x2F: {
        s32 special = 0;
        if (((actor->field_1E >> 2) & 1) || actor->field_1F == 0x1B) special = 1;
        if (special) {
            Unit *unit;
            s16 offset;
            s32 original = index;
            func_80084A20();
            func_800851B0(0x12)->field_1C = 12;
            func_80084B80();
            func_80049414(actor, &flags2, &mode2);
            func_80084A20();
            task = func_80085154(func_8008B004, index);
            func_80084B80();
            task->field_2C = 0;
            unit = func_8007946C(0, index);
            task->field_5C = flags2;
            task->field_60 = mode2;
            task->field_64 = unit->field_3C;
            if ((actor->field_1E >> 2) & 1) task->script_20 = D_8013EA70;
            else task->script_20 = D_8013EAA8;
            index = func_8007920C(item->kind, item->id, 10, 10);
            func_8004977C(index, original);
            unit = func_8007946C(4, index);
            offset = -10;
            if ((actor->field_1E >> 2) & 1) offset = -30;
            unit->field_0C = offset;
            task = func_80085154(func_8008B004, index);
            task->field_2C = 4;
            task->script_20 = D_8013E9F8;
        }
        break;
    }
    case 0x31: case 0x33:
        func_800851B0(0x18);
        /* Fall through to the inventory update. */
    case 0x3D: case 0x84:
        task = func_80085154(func_8008872C, index);
        task->field_24 = actor->field_0A;
        task->field_28 = item->kind;
        task->field_2C = item->id;
        if (task->field_28 == 3 && func_8010EBA4(item)) {
            task = func_80085154(func_8008876C, index);
            task->field_24 = actor->field_0A;
            task->field_28 = item->kind;
            task->field_2C = 1;
        }
        break;
    case 0x32: case 0x34:
        func_800851B0(0x19);
        /* Fall through to the inventory update. */
    case 0x85:
        task = func_80085154(func_8008872C, index);
        task->field_24 = actor->field_0A;
        task->field_28 = item->kind;
        task->field_2C = -1;
        break;
    case 0x37:
        switch (actor->field_1F) {
        case 0x17: {
            s32 sound = 0x69;
            if (item->id == 0x79) sound = 0x6A;
            else if (item->id == 0x7A) sound = 0x6B;
            func_800850F8(func_8008865C, 0);
            func_80084A20();
            func_800850F8(func_8008865C, 6);
            func_800851B0(sound);
            func_80084B80();
            func_80084A20();
            func_80050FA0(0x170, actor, actor->field_08, 0x200);
            func_80084B80();
            func_80051264(0x176, actor, 0x10200);
            break;
        }
        case 0x37:
            func_80051264(actor->field_75 == 1 ? 0xD5 : 0xD7, actor, 0x10000);
            break;
        case 0x38:
            func_80051264(0xAC, actor, 0);
            break;
        case 0x51:
            func_80051264(0x12E, actor, 0);
            break;
        case 0x54:
            func_80084A68();
            func_800850F8(func_8008B678, 0);
            func_80084A20();
            func_80050FA0(0x114, actor, actor->field_08, 0);
            func_80084B80();
            func_80051264(0x122, actor, 0);
            break;
        }
        break;
    case 0x8D:
        if (item->id == 0x4A) {
            task = func_80085154(func_8008876C, index);
            task->field_24 = actor->field_0A;
            task->field_28 = item->kind;
            task->field_2C = 1;
        }
        break;
    case 0x8E:
        if (item->id == 0x4A) {
            task = func_80085154(func_8008876C, index);
            task->field_24 = actor->field_0A;
            task->field_28 = item->kind;
            task->field_2C = 0;
        }
        break;
    }
}
