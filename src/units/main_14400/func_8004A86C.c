#include "common.h"

#define NULL ((void *)0)

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 x, y; } Pos;
typedef struct { u8 value; } Dir;

/* g++ 2.x vtable slot: this-adjustment delta, index, function pointer. */
typedef struct {
    s16 delta;
    s16 index;
    s32 (*fn)(void *self, s32 a, s32 b, u8 c, s32 d);
} VtblEntry;

/* Accessed prefix of the acting unit. */
typedef struct Actor {
    Pos pos;
    u8 direction;
    u8 field_09;
    u8 kind;
    u8 pad0B[0x13];
    u8 flags;
    u8 type;
    u8 pad20[4];
    VtblEntry *vtbl;
    u8 pad28[0x4A];
    u8 state;
    u8 pad73[2];
    u8 level;
    u8 pad76[0x6E];
    u16 status;
    u8 padE6[0x1E];
    struct Actor *retained;
} Actor;

/* Accessed prefix of the 0xB0-byte display unit returned by func_8007946C. */
typedef struct {
    u8 pad00[2];
    s16 kind;
    u8 pad04[4];
    u8 state;
    u8 mode;
    u16 flags;
    u8 pad0C[0x10];
    float scale_x;
    float scale_y;
    u8 pad24[0x18];
    u16 frame;
    u8 pad3E[0xA];
    u8 facing;
} Unit;

typedef struct { u8 kind, id; } Item;

/* 0x74-byte task record created by func_80085154. The interpreter at
 * func_8008B004 reads u16 script words. */
typedef struct {
    void (*handler)(void *);
    u8 pad04[0xE];
    u16 flags;
    s32 key;
    u8 pad18[8];
    const u16 *script;
    s32 a, b, c;
    u8 pad30[0x2C];
    s32 d, e, f, g;
    u8 pad6C[8];
} Action;

/* Equipment descriptor (4 bytes): +0 model/tile id. */
typedef struct { u16 id; u8 percent; u8 unk3; } EquipInfo;

typedef struct { u8 kind, variant, field_02, flags, row, field_05; u16 field_06; u8 mode; signed char coordinates[2], status; } SelectionRecord;

extern u32 D_8013960C, D_8013968C;
extern s32 D_80139690;
extern SelectionRecord D_80142F18;
extern EquipInfo D_8013F24C[], D_8013F2EC[];
/* Independent, 0x7FFF-terminated scripts. */
extern const u16 D_8013EAD8[], D_8013EB04[], D_8013EE34[], D_8013EE48[];
extern const u16 D_8013EE5C[], D_8013EE7C[], D_8013EE90[], D_8013EEB0[];
extern const u16 D_8013EED0[], D_8013EF58[], D_8013EFB4[], D_8013F01C[];
extern const u16 D_8013F058[], D_8013F0B4[], D_8013F12C[];

s32 func_80046240(void);
s32 func_80048AE4(void);
void func_80048FD8(void *);
void func_80049414(Actor *, u32 *, s32 *);
void func_8004977C(s32, s32);
s32 func_80049CB4(s32, ...);
void func_8004D588(Actor *, s32, s32);
void func_80050B3C(s32, Pos *, s32);
void func_80050E44(s32, Pos *);
void *func_80050EB4(s32, Pos *, s32, s32, s32, s32, s32);
void func_80050F34(s32, Pos *, s32, s32);
void *func_80050FA0(s32, Actor *, s32, s32);
void *func_800510E0(s32, Actor *, s32, s32, s32);
void func_80051208(s32, Actor *, s32);
void func_80051264(s32, Actor *, s32);
s32 func_8005CB7C(void);
s32 func_800625FC(s32, s32);
s32 func_800627C4(void);
s32 func_8007920C(s32, s32, s32, s32);
Unit *func_8007946C(s32, s32);
void func_80079560(s32, s32, s32);
void func_80084868(void *);
void func_800848D0(void *);
void func_80084A00(void);
void func_80084A20(void);
void func_80084A68(void);
Action *func_80084AB4(s32);
void func_80084B80(void);
u16 func_80084BCC(void);
s32 func_80084C00(s32, void (*)(void *), s32, s32, s32);
s32 func_80084C60(s32, void (*)(void *), u32);
void func_800850F8(void (*)(void *), u16);
Action *func_80085154(void (*)(void *), s32);
void *func_800851B0(s32);
void func_80085200(s32, s32, u16, s32);
void func_800853D4(void *);
void func_80085A80(void *);
void func_800868D8(void *);
void func_800869E8(void *);
void func_80086AC8(void *);
void func_80086B14(void *);
void func_80086B58(void *);
void func_80086C54(void *);
void func_80086CC0(void *);
void func_80086D8C(void *);
void func_800873C0(void *);
void func_8008767C(void *);
void func_80087830(void *);
void func_8008792C(void *);
void func_800879D4(void *);
void func_80087A98(void *);
void func_80087ABC(void *);
void func_80087B2C(void *);
void func_80087C20(void *);
void func_80087CBC(void *);
void func_80087E4C(void *);
void func_800885B8(void *);
void func_8008865C(void *);
void func_8008872C(void *);
void func_800887F0(void *);
void func_80088E38(void *);
void func_80088F98(void *);
void func_8008AEC8(void *);
void func_8008B004(void *);
void func_8008B678(void *);
void func_8008B814(void *);
void func_8008B994(void *);
void func_8008B9B0(void *);
void *func_800A2594(Pos *, Pos *, Dir);
u8 func_800A8C00(void *);
Actor *func_800C5F60(void);
s32 func_800E04D0(Actor *);
s32 func_800E1CC4(Actor *, s32);
s32 func_800E1CD4(Actor *, s32);
s32 func_800E2074(Actor *);
Item *func_800E8978(Actor *);
Item *func_800E8A68(Actor *, u8);
s32 func_8010BEC4(Item *, u8);
s32 func_8010EBA4(Item *);

static inline void get_pos(Pos *out, Actor *actor)
{
    out->x = actor->pos.x;
    out->y = actor->pos.y;
}

/* Equipped item's id byte. */
static inline u8 item_id(Item *item)
{
    return item->id;
}

/* ODD_C: the flooded-terrain predicate is a separate inline, as in func_8004D588. */
static inline s32 terrain_is_flooded(u32 terrain)
{
    s32 flooded = 0;
    if ((terrain & 0x20100000) && func_800627C4() == 3) {
        flooded = (D_80142F18.flags & 3) == 2;
    }
    return flooded;
}

/* Plays the presentation for the action recorded in D_8013968C on ACTOR:
 * sound cues (func_800851B0), effect tasks (func_80085154) and actor animation
 * effects (func_80051264/func_80050FA0/func_800510E0). INDEX is the actor's slot
 * from func_800A8C00; several arms reuse it for a freshly spawned effect object. */
void func_8004A86C(Actor *actor)
{
    Pos position;
    Pos front;
    u32 flags_1E;
    s32 mode_1E;
    u32 flags_1F;
    s32 mode_1F;
    u32 flags_3A;
    s32 mode_3A;
    u32 flags_3C;
    s32 mode_3C;
    u32 flags_3F;
    s32 mode_3F;
    u32 flags_6F;
    s32 mode_6F;
    u32 flags_88;
    s32 mode_88;
    u32 flags_94;
    s32 mode_94;
    u32 flags_A5;
    s32 mode_A5;
    s32 index = func_800A8C00(actor);
    Action *task;
    Unit *unit;

    switch (D_8013968C) {
    case 0x15:
        if (actor->flags & 0xC) {
            func_800851B0(0xB8);
        } else {
            func_800851B0(5);
        }
        break;

    case 0x1E: {
        Unit *unit;
        s32 type;
        func_800851B0(0x98);
        task = func_80085154(func_800873C0, index);
        func_80049414(actor, &flags_1E, &mode_1E);
        unit = func_8007946C(0, index);
        task->d = flags_1E;
        task->e = mode_1E;
        task->f = unit->frame;
        type = actor->type;
        if (type == 0x17 || type == 0x1A) {
            task->flags |= 0x1000;
        }
        break;
    }

    case 0x1F: {
        Actor *self;
        s32 frozen;
        s32 slot;
        s32 blocked;
        s32 stuck;
        if (actor == NULL || !(actor->flags & 0x7C)) {
            break;
        }
        /* ODD_C: receiver alias for the slot-18 virtual query and func_800E04D0,
         * as in func_80049414 (self = obj). Using ACTOR directly, or an inline
         * predicate, allocates the receiver to the actor's register. */
        self = actor;
        frozen = 0;
        if ((actor->flags >> 2) & 1) {
            frozen = (actor->status >> 8) & 1;
        }
        if (frozen) {
            break;
        }
        if (actor == func_800C5F60()) {
            s32 facing = -1;
            if (func_800E1CC4(actor, 0) != 0) {
                if (func_8005CB7C() == 0) {
                    facing = 1;
                }
            } else if (func_8005CB7C() == 1) {
                facing = 0;
            }
            if (facing >= 0) {
                func_80085154(func_8008B814, index)->a = facing;
            }
        }
        unit = func_8007946C(0, index);
        if (index == 0xFF || actor->type == 0) {
            break;
        }
        func_80049414(actor, &flags_1F, &mode_1F);
        slot = func_80084C60(func_80084BCC(), func_8008B994, index);
        if ((D_8013960C & 1) && unit->kind != 0xD6 && !(unit->flags & 4) && (flags_1F & 4)) {
            if (slot >= 0 && !(func_80084AB4(slot)->a & 4)) {
                slot = -1;
            }
            if (slot < 0) {
                get_pos(&position, actor);
                func_80050EB4(0x1C8, &position, 2, 0, 0, 0, 0x2C);
            }
        }
        task = func_80085154(func_8008B994, index);
        blocked = 0;
        task->a = flags_1F;
        task->b = mode_1F;
        if (self->vtbl[18].fn((u8 *)self + self->vtbl[18].delta, 2, 0x11, 0, 0) == 0 || (u8)func_800E04D0(self)) {
            blocked = 1;
        }
        task->c = blocked;
        if (mode_1F == 3 && unit->mode != mode_1F) {
            func_80085154(func_800869E8, 0);
        }
        if ((D_8013960C & 1) && unit->kind != 0xD6 && (unit->flags & 4)) {
            stuck = 0;
            if (self->vtbl[18].fn((u8 *)self + self->vtbl[18].delta, 2, 0x11, 0, 0) == 0 || (u8)func_800E04D0(self)) {
                stuck = 1;
            }
            if (stuck) {
                if (slot >= 0 && func_80084AB4(slot)->c != 0) {
                    slot = -1;
                }
                if (slot < 0) {
                    get_pos(&position, actor);
                    func_80050EB4(0x1C2, &position, 2, 0, 0, 0, 0x2C);
                }
            }
        }
        break;
    }

    case 0xA7:
        if (actor == func_800C5F60() && func_8005CB7C() == 1) {
            func_80085154(func_8008B814, index)->a = 0;
        }
        task = func_80085154(func_8008B9B0, index);
        task->a = 0;
        task->b = 5;
        break;

    case 0x27:
        get_pos(&position, actor);
        if (!(func_800625FC(position.y, position.x) & 0x2000)) {
            func_800851B0(0xD);
        }
        break;

    case 0x28:
        if ((actor->flags >> 2) & 1) {
            func_80051208(0x161, actor, 0x10000);
            func_800851B0(0xA9);
        }
        break;

    case 0x29:
        switch (actor->type) {
        case 0x17:
            func_80084A20();
            func_80050FA0(0x171, actor, actor->direction, 0x10000);
            func_80084B80();
            func_80051264(0x17B, actor, 0x10000);
            break;
        case 0x1B:
            func_80051264(0x100, actor, 0x40000);
            break;
        case 0x40:
            func_80051264(0x100, actor, 0);
            break;
        case 0x39:
            func_80051264(0x126, actor, 0);
            break;
        case 0x54:
            func_80084A68();
            func_800850F8(func_8008B678, 0);
            func_80084A20();
            func_80050FA0(0x114, actor, actor->direction, 0);
            func_80084B80();
            func_80051264(0x122, actor, 0);
            break;
        }
        break;

    case 0x2D: {
        s32 original;
        get_pos(&position, actor);
        func_800850F8(func_8008865C, 7);
        func_80084A20();
        func_80050E44(0x142, &position);
        func_80084B80();
        func_800850F8(func_8008865C, 0x17);
        func_800851B0(0x83);
        func_80085154(func_80087B2C, index);
        original = index;
        index = func_8007920C(9, 0xA6, 10, 10);
        func_8004977C(index, original);
        task = func_80085154(func_8008B004, index);
        task->c = 4;
        task->script = D_8013EF58;
        break;
    }

    case 0x2E: {
        s32 original;
        get_pos(&position, actor);
        func_800850F8(func_8008865C, 7);
        func_80084A20();
        func_80050E44(0x142, &position);
        func_80084B80();
        func_800850F8(func_8008865C, 0x17);
        func_800851B0(0x1C);
        func_80085154(func_80087B2C, index);
        original = index;
        index = func_8007920C(9, 0xA6, 10, 10);
        func_8004977C(index, original);
        task = func_80085154(func_8008B004, index);
        task->c = 4;
        task->script = D_8013EFB4;
        break;
    }

    case 0x38:
        switch (actor->type) {
        case 0x18:
            func_80051264(0x13B, actor, 0);
            break;
        case 0x26:
            func_80051264(0xCE, actor, 0);
            break;
        case 0x50:
            func_80051264(0x12C, actor, 0);
            break;
        }
        break;

    case 0x39:
        switch (actor->type) {
        case 0x17:
            func_80084A20();
            func_80050FA0(0x16A, actor, actor->direction, 0x10000);
            func_80084B80();
            func_80051264(0x156, actor, 0x10000);
            break;
        case 0x1B:
            func_80051264(0x9A, actor, 0x40000);
            break;
        }
        break;

    case 0x3A:
    case 0x3B:
        func_80084A20();
        func_800850F8(func_8008865C, 4);
        func_800851B0(0x27);
        func_80084B80();
        if ((actor->flags >> 2) & 1) {
            s32 type = actor->type;
            s32 original;
            Unit *unit;
            func_80049414(actor, &flags_3A, &mode_3A);
            original = index;
            func_80084A20();
            task = func_80085154(func_8008B004, index);
            func_80084B80();
            task->c = 0;
            unit = func_8007946C(0, index);
            task->d = flags_3A;
            task->e = mode_3A;
            task->f = unit->frame;
            if (type == 0x17) {
                task->script = D_8013EE5C;
            } else {
                task->script = D_8013EE90;
            }
            index = func_8007920C(0, 0x168, 10, 10);
            func_8004977C(index, original);
            task = func_80085154(func_8008B004, index);
            task->c = 4;
            if (type == 0x17) {
                task->script = D_8013EE34;
            } else {
                task->script = D_8013EE7C;
            }
        }
        break;

    case 0x3C: {
        s32 original;
        Unit *unit;
        func_80049414(actor, &flags_3C, &mode_3C);
        func_800851B0(0x7F);
        task = func_80085154(func_8008B004, index);
        task->c = 0;
        task->script = D_8013EE5C;
        unit = func_8007946C(0, index);
        task->d = flags_3C;
        task->e = mode_3C;
        task->f = unit->frame;
        original = index;
        index = func_8007920C(0, 0x168, 10, 10);
        func_8004977C(index, original);
        task = func_80085154(func_8008B004, index);
        task->c = 4;
        task->script = D_8013EE48;
        break;
    }

    case 0x3E:
        task = func_80085154(func_80086B58, index);
        get_pos(&position, actor);
        task->c = func_8007920C(9, 0xAC, position.y, position.x);
        func_80079560(4, task->c, 1);
        break;

    case 0x3F: {
        s32 original;
        Unit *unit;
        func_800851B0(0x25);
        func_80049414(actor, &flags_3F, &mode_3F);
        task = func_80085154(func_8008B004, index);
        task->c = 0;
        task->script = D_8013EEB0;
        unit = func_8007946C(0, index);
        task->d = flags_3F;
        task->e = mode_3F;
        task->f = unit->frame;
        original = index;
        index = func_8007920C(0, 0x167, 10, 10);
        func_8004977C(index, original);
        task = func_80085154(func_8008B004, index);
        task->c = 4;
        task->script = D_8013EED0;
        break;
    }

    case 0x42:
        func_800851B0(0x8F);
        if (actor->type == 0x17) {
            if (func_80048AE4() != 0) {
                func_80085154(func_800868D8, 4);
            }
            func_80084A20();
            func_80050FA0(0x172, actor, actor->direction, 0x10000);
            func_80084B80();
            func_80051264(0x17C, actor, 0x10000);
        } else {
            func_80085200(index, 0, 0xC, 0);
        }
        break;

    case 0x46: {
        s32 turn;
        get_pos(&position, actor);
        turn = actor->direction + 4;
        func_80050F34(0x133, &position, turn % 8, 0);
        break;
    }

    case 0x43:
        get_pos(&position, actor);
        func_80050E44(0x144, &position);
        break;

    case 0x40:
        switch (actor->type) {
        case 0x17: {
            s32 flags = 0x10000;
            s32 slow;
            Item *item = func_800E8A68(actor, 4);
            /* ODD_C: the id is read through a separate handle (the ROM copies the
             * pointer before the NULL test); reading through ITEM drops the copy. */
            Item *weapon = item;
            s32 id;
            s32 effect;
            s32 terrain;
            if (item == NULL) {
                id = 0;
            } else {
                id = item_id(weapon);
            }
            effect = 0;
            if (id != 0) {
                effect = 0x177;
            }
            slow = flags | 0x800;
            func_80084A68();
            func_800850F8(func_8008B678, 0);
            func_80084A20();
            func_80051264(0x14A, actor, slow);
            func_80084B80();
            func_80084A20();
            func_80050FA0(0x162, actor, actor->direction, slow);
            func_80084B80();
            get_pos(&position, actor);
            terrain = func_800625FC(position.y, position.x);
            if ((terrain & 0x4000) || terrain_is_flooded(terrain)) {
                flags |= 0x40;
            }
            if (effect != 0) {
                s32 tile = D_8013F2EC[id - 0x59].id;
                func_80084A20();
                func_800510E0(effect, actor, actor->direction, flags | 0x800, tile);
                func_80084B80();
            }
            func_80084A20();
            func_800510E0(0x17A, actor, actor->direction, flags | 0x400, 0);
            func_80084B80();
            break;
        }
        case 0x1B:
            func_80051264(0x9B, actor, 0x40000);
            break;
        case 0x36:
            func_80051264(0xE2, actor, 0);
            break;
        case 0x3B:
            func_80051264(0xE4, actor, 0);
            break;
        case 0x43:
            func_80051264(0x104, actor, 0);
            break;
        }
        break;

    case 0x4B:
        func_80051264(0xBF, actor, 0);
        break;
    case 0x4C:
        func_80051264(0x117, actor, 0);
        break;
    case 0x4D:
        func_80051264(0xC3, actor, 0);
        break;
    case 0x4E:
        func_80051264(0xC9, actor, 0);
        break;
    case 0x4F:
        func_80051264(0x10B, actor, 0);
        break;
    case 0x50:
        func_80051264(0xF3, actor, 0);
        break;
    case 0x51:
        func_80051264(0xFC, actor, 0);
        break;
    case 0x52:
    case 0x53:
        switch (actor->level) {
        case 1:
            func_80051264(0xEF, actor, 0);
            break;
        case 2:
            func_80051264(0xF0, actor, 0);
            break;
        case 3: /* ODD_C: level 3 (the level after 1 and 2) uses the same 0xF1 effect
                 * as any other level. Listing it makes the case set {1,2,3},
                 * whose balanced compare tree (==2, <3, ==1) is the ROM's. Measured
                 * without this label: 1157 differing words (object 7984/7992 bytes;
                 * the arm becomes an ==1/==2 chain, 10 words); an if/else chain
                 * instead of the switch: 1163 words. */
        default:
            func_80051264(0xF1, actor, 0);
            break;
        }
        break;
    case 0x54:
    case 0x55:
        func_80051264(0xC7, actor, 0);
        break;
    case 0x56:
        func_80051264(0xC1, actor, 0);
        break;
    case 0x57:
        func_80051264(0xAE, actor, 0);
        break;
    case 0x58:
        func_80051264(0x119, actor, 0);
        break;
    case 0x59:
        func_80051264(0xD1, actor, 0);
        break;
    case 0x5A:
        func_80051264(0x11F, actor, 0x10000);
        break;
    case 0x5B:
        func_80051264(0xFE, actor, 0);
        break;
    case 0x5C:
        switch (actor->type) {
        case 0x2C:
            func_80051264(0xD3, actor, 0);
            break;
        case 0x53:
            func_80051264(0xFA, actor, 0x20000);
            break;
        }
        break;
    case 0x5E:
        func_80051264(0xEB, actor, 0);
        break;
    case 0x5F:
        func_80051264(0x11B, actor, 0);
        break;
    case 0x60:
        func_80051264(0x102, actor, 0);
        break;
    case 0x61:
        func_80051264(0x12A, actor, 0);
        break;
    case 0x62:
        func_80051264(0x129, actor, 0);
        break;
    case 0x63:
        func_80051264(0x36, actor, 4);
        break;

    case 0x41: {
        s32 ready = 0;
        if (actor->flags & 0xC) {
            Item *item = func_800E8A68(actor, 4);
            s32 usable = 0;
            if (item != NULL && (u8)func_8010BEC4(item, 0x67) && func_800E1CD4(actor, 0xD) == 0
                && func_800E1CD4(actor, 0xA) == 0 && func_800E1CD4(actor, 0xB) == 0
                && !(actor->state & 1) && func_800E1CD4(actor, 0xE) == 0
                && (!((actor->flags >> 2) & 1) || actor->retained == NULL)) {
                usable = 1;
            }
            if (usable) {
                ready++;
            }
        }
        if (ready == 0) {
            func_80085154(func_8008B678, 0);
        } else {
            Item *shield = func_800E8A68(actor, 3);
            Item *weapon = func_800E8A68(actor, 4);
            s32 type = actor->type;
            s32 direction = actor->direction;
            /* ODD_C: narrow short/char copies of the type, as the ROM tests and
             * selects through two separate copies of the loaded byte. */
            u16 hero = type;
            if (hero == 0x17 || hero == 0x1A) {
                u8 kind = type;
                s32 body = kind == 0x17 ? 0x173 : 0xB;
                s32 weapon_effect = kind == 0x17 ? 0x174 : 0xC;
                s32 shield_effect = kind == 0x17 ? 0x175 : 0xD;
                s32 swing = kind == 0x17 ? 0x16F : 0xA;
                func_80084A00();
                func_80084A20();
                task = func_80085154(func_80085A80, 0);
                task->flags |= 0x400;
                func_80084B80();
                func_80084A00();
                func_80084A20();
                func_80051264(body, actor, 0x10800);
                func_80084B80();
                func_80084A00();
                func_80084A20();
                func_80050FA0(swing, actor, direction, 0x10800);
                func_80084B80();
                if (weapon != NULL) {
                    s32 tile = D_8013F2EC[weapon->id - 0x59].id;
                    func_80084A00();
                    func_80084A20();
                    func_800510E0(weapon_effect, actor, direction, 0x10800, tile);
                    func_80084B80();
                }
                if (shield != NULL) {
                    s32 tile = D_8013F24C[shield->id - 0x32].id;
                    func_80084A00();
                    func_80084A20();
                    func_800510E0(shield_effect, actor, direction, 0x10800, tile);
                    func_80084B80();
                }
            }
        }
        break;
    }

    case 0x65:
    case 0x66: {
        s32 type = actor->type;
        s32 direction = actor->direction;
        if ((func_800E2074(actor) ^ 1) == 0 || (((actor->flags >> 2) & 1) && actor->retained == NULL)) {
            switch (type) {
            case 0x1A:
            case 0x17: {
                s32 kind = actor->type;
                s32 body = kind == 0x17 ? 0x173 : 0xB;
                s32 weapon_effect = kind == 0x17 ? 0x174 : 0xC;
                s32 shield_effect = kind == 0x17 ? 0x175 : 0xD;
                s32 swing = kind == 0x17 ? 0x16F : 0xA;
                Item *shield = func_800E8A68(actor, 3);
                Item *weapon = func_800E8A68(actor, 4);
                func_80084A68();
                if (D_8013968C == 0x66) {
                    func_80084A20();
                    func_800850F8(func_8008865C, 8);
                    func_800851B0(0x68);
                    func_80084B80();
                }
                func_800850F8(func_8008865C, 0);
                func_80084A20();
                func_80051264(body, actor, 0x10200);
                func_80084B80();
                func_80084A20();
                func_80050FA0(swing, actor, direction, 0x10200);
                func_80084B80();
                if (weapon != NULL) {
                    s32 tile = D_8013F2EC[weapon->id - 0x59].id;
                    func_80084A20();
                    func_800510E0(weapon_effect, actor, direction, 0x10200, tile);
                    func_80084B80();
                }
                if (shield != NULL) {
                    s32 tile = D_8013F24C[shield->id - 0x32].id;
                    func_80084A20();
                    func_800510E0(shield_effect, actor, direction, 0x10200, tile);
                    func_80084B80();
                }
                break;
            }
            case 0x46:
                func_80051264(0x106, actor, 0x200);
                if (D_8013968C == 0x66) {
                    func_80084A20();
                    func_800850F8(func_8008865C, 8);
                    func_800851B0(0x68);
                    func_80084B80();
                }
                break;
            case 0x3C:
                func_80051264(0x10D, actor, 0);
                break;
            }
        }
        break;
    }

    case 0x68:
        if (D_80139690 != 0) {
            func_80051264(0xE8, actor, 0);
        } else {
            get_pos(&position, actor);
            func_80050B3C(0xE8, &position, 0x19);
        }
        break;

    case 0x69:
        switch (actor->type) {
        case 0x17:
            func_8004D588(actor, 1, 0);
            break;
        case 0x41:
            func_80051264(0xE6, actor, 0);
            break;
        }
        break;

    case 0x6A: {
        s32 flags = 0x20000;
        if (actor->level < 4) {
            flags = 0x10000;
        }
        func_80051264(0xD9, actor, flags);
        break;
    }

    case 0x6B:
        switch (actor->type) {
        case 0x19:
            func_80051264(0xA0, actor, 0x10000);
            break;
        case 0x1E:
            func_80051264(0xBD, actor, 0);
            break;
        }
        break;

    case 0x6C:
        switch (actor->type) {
        case 0x47:
            func_80051264(0xB0, actor, 0);
            break;
        case 0x49:
            if ((actor->field_09 & 0xF) == 2) {
                func_80051264(0xB8, actor, 0);
            } else {
                func_80051264(0xB7, actor, 0);
            }
            break;
        }
        break;

    case 0x6D:
        switch (actor->type) {
        case 0x4F:
            func_80051264(0xE0, actor, 0);
            break;
        case 0x56:
            func_80051264(0x128, actor, 0);
            break;
        }
        break;

    case 0x6E:
        switch (actor->type) {
        case 0x17:
        case 0x1A:
            func_8004D588(actor, 1, 0);
            break;
        }
        break;

    case 0x6F: {
        s32 hero = 0;
        if ((actor->flags >> 2) & 1) {
            hero = actor->type == 0x17;
        }
        if (hero) {
            func_80049414(actor, &flags_6F, &mode_6F);
            unit = func_8007946C(0, index);
            task = func_80085154(func_80087C20, index);
            task->a = flags_6F;
            task->b = mode_6F;
            task->c = unit->frame;
        } else {
            func_80085200(index, 0, 0xC, 0);
        }
        break;
    }

    case 0x70:
        if (actor == NULL) {
            break;
        }
        if ((actor->flags >> 2) & 1) {
            Unit *source = func_8007946C(0, index);
            if (source->state == 0) {
                task = func_80085154(func_8008B004, index);
                task->c = 0;
                task->script = D_8013F01C;
                index = func_8007920C(0, 0x145, actor->pos.y, actor->pos.x);
                unit = func_8007946C(4, index);
                func_80079560(4, index, 1);
                unit->scale_x = unit->scale_y = 0.3f;
                unit->facing = source->facing;
                task = func_80085154(func_8008B004, index);
                task->c = 4;
                task->script = D_8013F058;
            }
        } else {
            if (actor->type == 0x5B) {
                task = func_80085154(func_8008792C, index);
            } else {
                task = func_80085154(func_80087830, index);
                task->b = func_80046240();
            }
            task->a = actor->kind;
        }
        break;

    case 0x71:
        func_800851B0(0x71);
        func_80085154(func_800879D4, index);
        break;

    case 0x72:
        get_pos(&position, actor);
        if (func_80084C00(func_80084BCC(), func_800887F0, position.y, position.x, 1) >= 0) {
            func_800850F8(func_8008865C, 8);
        }
        func_80050E44(0x34, &position);
        break;

    case 0x73: {
        s32 scripted = 0;
        if (actor->type == 0x17) {
            task = func_80085154(func_8008B004, index);
            scripted = 1;
            task->c = 0;
            task->script = D_8013EB04;
        }
        func_800851B0(0x94);
        task = func_80085154(func_80087E4C, index);
        if (scripted) {
            task->flags = 0x4000;
        }
        break;
    }

    case 0x74: {
        s32 scripted = 0;
        s32 type = actor->type;
        func_80085154(func_800853D4, 0);
        if (type == 0x17) {
            task = func_80085154(func_8008B004, index);
            scripted = 1;
            task->c = 0;
            task->script = D_8013EAD8;
        }
        task = func_80085154(func_80087CBC, index);
        if (scripted) {
            task->flags = 0x4000;
        }
        break;
    }

    case 0x77:
        get_pos(&position, actor);
        func_800850F8(func_8008865C, 7);
        func_80084A20();
        func_80050E44(0x142, &position);
        func_80084B80();
        func_800850F8(func_8008865C, 0xD);
        func_80084A20();
        func_80050E44(0x136, &position);
        func_800851B0(0x85);
        func_80084B80();
        func_80085154(func_80086D8C, index);
        break;

    case 0x80:
        get_pos(&position, actor);
        func_80050E44(0x1B1, &position);
        break;

    case 0x86:
        func_80085154(func_80087A98, index);
        break;

    case 0x88:
        if (actor != NULL && index != 0xFF && actor->type != 0) {
            s32 level = 1;
            if (actor->flags & 0x7C) {
                level = actor->level;
            }
            task = func_80085154(func_80087ABC, index);
            task->a = actor->type;
            task->b = level;
            task->c = actor->direction;
            task->d = actor->pos.y;
            task->g = actor->pos.x;
            if (actor->flags & 0x7C) {
                func_80049414(actor, &flags_88, &mode_88);
                task = func_80085154(func_8008B9B0, index);
                task->a = flags_88;
                task->b = mode_88;
            }
        }
        break;

    case 0x87:
        func_80085154(func_800885B8, index)->a = actor->kind;
        break;

    case 0x89:
        func_80048FD8(func_800C5F60());
        break;

    case 0x8B:
        func_80085154(func_80088F98, index)->a = actor->direction;
        break;

    case 0x90:
        func_80085154(func_80086C54, index);
        break;

    case 0x91:
        func_80085154(func_80088E38, index);
        task = func_80085154(func_8008B9B0, index);
        task->a = 0;
        task->b = 0;
        break;

    case 0x93:
        func_80085154(func_80086B14, index);
        break;

    case 0x94: {
        s32 hero = 0;
        Unit *unit;
        if ((actor->flags >> 2) & 1) {
            hero = actor->type == 0x17;
        }
        if (hero) {
            func_80049414(actor, &flags_94, &mode_94);
            task = func_80085154(func_8008B004, index);
            unit = func_8007946C(0, index);
            task->c = 0;
            task->script = D_8013F0B4;
            task->d = flags_94;
            task->e = mode_94;
            task->f = unit->frame;
        }
        break;
    }

    case 0x95:
        func_80084A20();
        func_800850F8(func_8008865C, 2);
        func_800851B0(0x132);
        func_80084B80();
        func_80085154(func_8008767C, index)->a = actor->direction;
        break;

    case 0x96:
        task = func_80085154(func_80086CC0, index);
        if (actor == func_800C5F60()) {
            task->flags = 2;
        }
        break;

    case 0x97:
        func_80051264(0xB3, actor, 0x80);
        break;
    case 0x98:
        func_80051264(0xB4, actor, 0);
        break;
    case 0x99:
        func_80051264(0xB2, actor, 0);
        break;

    case 0x9A:
        switch (actor->type) {
        case 0x17:
            func_80051264(0x14A, actor, 0x10000);
            break;
        case 0x1B:
            func_80051264(0x9C, actor, 0x40000);
            break;
        }
        func_800851B0(0x23);
        break;

    case 0x9B:
        func_80051264(0x130, actor, 0);
        break;

    case 0x9D:
        if (!((D_80142F18.flags >> 2) & 1)) {
            s32 type = actor->type;
            if (type == 0x17 || type == 0x1A) {
                Item *item = func_800E8A68(actor, 3);
                if (item != NULL) {
                    task = func_80085154(func_8008872C, index);
                    task->a = actor->kind;
                    task->b = 3;
                    task->c = item->id;
                    if (func_8010EBA4(item) != 0) {
                        func_80049CB4(0x8D, actor, item);
                    }
                }
                item = func_800E8A68(actor, 4);
                if (item != NULL) {
                    task = func_80085154(func_8008872C, index);
                    task->a = actor->kind;
                    task->b = 4;
                    task->c = item->id;
                }
                item = func_800E8978(actor);
                if (item != NULL) {
                    s32 kind = item->kind;
                    if (kind == 9) {
                        task = func_80085154(func_8008872C, index);
                        task->a = actor->kind;
                        task->b = kind;
                        task->c = item->id;
                    }
                }
            }
        }
        break;

    case 0xA0: {
        Dir facing;
        func_80051264(0x157, actor, 0x10000);
        get_pos(&position, actor);
        facing.value = actor->direction;
        func_800A2594(&front, &position, facing);
        task = func_80085154(func_8008AEC8, -1);
        task->d = front.y;
        task->g = front.x;
    }
        /* fallthrough */
    case 0xA1:
        func_800851B0(0x97);
        break;

    case 0xA3: {
        s32 radius;
        get_pos(&position, actor);
        radius = actor->level - 1;
        func_800851B0(0x138);
        func_80050F34(0x139, &position, 2, radius);
        break;
    }

    case 0xA4: {
        s32 radius;
        get_pos(&position, actor);
        radius = actor->level - 1;
        func_800851B0(0x139);
        func_80050F34(0x139, &position, 0, radius);
        break;
    }

    case 0xA5: {
        Unit *unit = func_8007946C(0, index);
        func_80049414(actor, &flags_A5, &mode_A5);
        func_80085154(func_800868D8, 4);
        func_800851B0(0x11);
        task = func_80085154(func_8008B004, index);
        task->c = 0;
        task->script = D_8013F12C;
        task->d = flags_A5;
        task->e = mode_A5;
        task->f = unit->frame;
        break;
    }

    case 0xA8:
        task = func_80085154(func_80086D8C, index);
        task->flags |= 0x2000;
        break;

    case 0xA9:
        func_80085154(func_80086AC8, index);
        break;

    case 0xAA: {
        s32 ok = 0;
        if ((actor->flags >> 3) & 1) {
            ok = actor->kind != 0x1C;
        }
        if (ok) {
            func_80085154(func_80084868, index);
        }
        break;
    }

    case 0xAB: {
        s32 ok = 0;
        if ((actor->flags >> 3) & 1) {
            ok = actor->kind != 0x1C;
        }
        if (ok) {
            func_80085154(func_800848D0, index);
        }
        break;
    }

    case 0xAC:
        func_80051264(0x7F, actor, 0x10000);
        break;
    }
}
