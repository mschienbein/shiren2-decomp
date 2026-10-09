#include "common.h"
typedef struct { s32 x; s32 y; } Pos;
typedef struct {
    Pos pos;
    unsigned char f8;
    unsigned char f9;
    unsigned char fA;
    char padB[0x1E - 0xB];
    unsigned char f1E;
    unsigned char f1F;
    char pad20[0x75 - 0x20];
    unsigned char f75;
} Unit;
typedef struct {
    char pad0[0x12];
    unsigned short flags;
    char pad14[0x24 - 0x14];
    s32 f24;
    s32 f28;
    s32 f2C;
    char pad30[0x5C - 0x30];
    s32 f5C;
    s32 f60;
    char pad64[0x68 - 0x64];
    s32 f68;
    s32 f6C;
} Effect;
typedef struct { unsigned char f0; unsigned char f1; } ItemHead;
typedef void (*EffectFn)(void *);
extern u32 D_8013968C;
void func_80086E94(void *task);
void func_8008880C(void *task);
void func_800887F0(void *task);
void func_8008A198(void *task);
void func_8008A474(void *task);
void func_8008B9B0(void *task);
void func_8008865C(void *task);
void func_80087FEC(void *task);
void func_80088380(void *task);
unsigned char func_800A8C00(void *actor);
void *func_800851B0(s32 id);
void *func_80085154(EffectFn fn, s32 layer);
void func_800850F8(EffectFn fn, unsigned short value);
Unit *func_800C5F60(void);
void func_8004D588(Unit *u, s32 a, s32 b);
ItemHead *func_800E8978(Unit *u);
s32 func_800E0F40(Unit *u);
void func_80049414(Unit *u, u32 *flags, s32 *mode);
s32 func_80048EE0(Unit *u);
void func_80050E44(s32 id, Pos *pos);
static inline void Pos_copy(Pos *dst, Pos *src) {
    dst->x = src->x;
    dst->y = src->y;
}
void func_8004C91C(Unit *u, Pos *pos) {
    s32 layer = func_800A8C00(u);
    Effect *e;
    switch (D_8013968C) {
    case 0x1C:
    case 0x1D:
        func_800851B0(0x1E);
        e = func_80085154(func_80086E94, layer);
        e->f5C = pos->y;
        e->f68 = pos->x;
        e->f60 = u->pos.y;
        e->f6C = u->pos.x;
        if (u->f1F == 0x53) {
            e->flags |= 0x2000;
        }
        if (D_8013968C == 0x1D) {
            e->flags |= 0x1000;
        }
        if (u == func_800C5F60()) {
            e->flags |= 2;
        }
        break;
    case 0x48:
        func_8004D588(u, 1, 0);
        break;
    case 0x49: {
        ItemHead *item = func_800E8978(u);
        if (item != 0 && item->f1 == 0x44) {
            e = func_80085154(func_800887F0, layer);
        } else {
            e = func_80085154(func_8008880C, layer);
        }
        e->f5C = u->pos.y;
        e->f68 = u->pos.x;
        e->f60 = pos->y;
        e->f6C = pos->x;
        if (u == func_800C5F60()) {
            e->flags |= 2;
        }
        e->flags |= 0x1000;
        break;
    }
    case 0x75: {
        Pos from;
        Pos_copy(&from, &u->pos);
        e = func_80085154(func_8008A198, layer);
        e->f5C = from.y;
        e->f68 = from.x;
        e->f60 = pos->y;
        e->f6C = pos->x;
        break;
    }
    case 0x78: {
        u32 a;
        s32 b;
        func_800851B0(0x34);
        e = func_80085154(func_8008A474, layer);
        e->f5C = pos->y;
        e->f68 = pos->x;
        e->f24 = u->f1F;
        e->f28 = (unsigned char)func_800E0F40(u);
        e->f2C = u->f8;
        e->f60 = u->pos.y;
        e->f6C = u->pos.x;
        func_80049414(u, &a, &b);
        e = func_80085154(func_8008B9B0, layer);
        e->f24 = a;
        e->f28 = b;
        break;
    }
    case 0x14: {
        s32 level;
        func_800851B0(0xFE);
        {
            s32 special = 0;
            if (u->f1F == 0x53) {
                special = func_80048EE0(u) != 0;
            }
            if (special) {
                func_800850F8(func_8008865C, 8);
                break;
            }
        }
        level = u->f75;
        e = func_80085154(func_80087FEC, layer);
        e->f24 = u->fA;
        e->f28 = level;
        e->f2C = u->f8;
        e->f5C = pos->y;
        e->f68 = pos->x;
        break;
    }
    case 0x82: {
        s32 level = 1;
        if (u->f1E & 0x7C) {
            level = u->f75;
        }
        e = func_80085154(func_80088380, layer);
        e->f24 = u->fA;
        e->f28 = level;
        e->f2C = u->f8;
        e->f5C = pos->y;
        e->f68 = pos->x;
        func_80050E44(0x1CC, pos);
        break;
    }
    }
}
