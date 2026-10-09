#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef short s16;

typedef struct {
    s8 x;
    s8 y;
} BytePair;

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    u8 value;
} Dir;

/* 0x20-byte message sent to objects hit by the blast. */
typedef struct {
    s32 type;
    u8 pad4[8];
    Dir dir;        /* 0x0C */
    u8 padD[3];
    Pos pos;        /* 0x10 */
    u8 pad18[8];
} Msg;

typedef struct {
    u8 pad0[0x38];
    s16 delta_38;
    s16 pad3A;
    s32 (*func_3C)(void *self, Msg *msg);
} ItemVTable;

typedef struct {
    u8 type;        /* 0x00 */
    u8 kind;        /* 0x01 */
    u8 flags2;      /* 0x02 */
    u8 pad3[5];
    ItemVTable *vtable_8;
    u8 flagsC;      /* 0x0C */
} Item;

extern s8 D_801486CC[8]; /* per-direction y offsets */
extern s8 D_801486D4[8]; /* per-direction x offsets */

s32 func_800C94D8(void);
Item *func_800B4D80(Pos *p);
s32 func_80049CB4(s32 id, ...);
void func_80115E18(Item *obj, Pos *position);
void *func_800A27A4(Dir *out, Pos *from, Pos *to);

static inline void get_dir_copy(Dir *dst, Dir src)
{
    *dst = src;
}

static inline void set_pos(Pos *pos, s32 x, s32 y)
{
    pos->x = x;
    pos->y = y;
}

/* Blast around `center`: hit sealed-free pots in the 8 neighbours, then notify them. */
void func_80116508(BytePair *center)
{
    Pos origin;
    Pos pos;
    u8 hit[8];
    Item *here;
    s32 busy;
    s32 i;
    s32 j;

    busy = func_800C94D8() != 1;
    if (busy) {
        return;
    }
    set_pos(&origin, center->x, center->y);
    for (i = 0;; i++) {
        Item *item;
        s32 ok;

        if (i >= 8) {
            break;
        }
        pos.y = center->y + D_801486CC[i];
        pos.x = center->x + D_801486D4[i];
        hit[i] = 0;
        item = func_800B4D80(&pos);
        ok = 0;
        if (item != 0 && item->type == 0x10) {
            s32 sealed = (item->flagsC >> 2) & 1;

            ok = sealed == 0;
        }
        if (ok) {
            func_80049CB4(0xED, &origin, &pos);
            if (item->flags2 & 0x10) {
                item->flags2 &= ~0x10;
                func_80049CB4(0xD7, &pos);
            }
            func_80049CB4(0x129, 2);
            hit[i] = 1;
        }
    }
    here = func_800B4D80(&origin);
    if (here != 0 && here->kind == 0xE8) {
        func_80115E18(here, &origin);
    }
    func_80049CB4(2);
    for (j = 0;; j++) {
        Pos target;
        Msg msg;
        Item *item;
        Dir found;
        Dir dir;

        if (j >= 8) {
            break;
        }
        if (hit[j] == 0) {
            continue;
        }
        pos.y = center->y + D_801486CC[j];
        pos.x = center->x + D_801486D4[j];
        target.x = pos.x;
        target.y = pos.y;
        func_800A27A4(&found, &origin, &target);
        get_dir_copy(&dir, found);
        item = func_800B4D80(&pos);
        if (item != 0) {
            msg.type = 0x15;
            msg.pos = pos;
            msg.dir = dir;
            item->vtable_8->func_3C((u8 *)item + item->vtable_8->delta_38, &msg);
        }
    }
}
