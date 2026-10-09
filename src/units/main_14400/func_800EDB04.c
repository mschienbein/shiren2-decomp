#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Vec;
typedef struct Obj {
    Vec pos;                /* 0x000 */
    char pad08[2];
    u8 bA;                  /* 0x00A */
    char pad0B[0x13];
    u8 b1E;                 /* 0x01E */
    char pad1F[9];
    u16 h28;                /* 0x028 */
    char pad2A[0x42];
    s32 v6C;                /* 0x06C */
    char pad70[0x74];
    u16 flags;              /* 0x0E4 */
    char padE6[0x1E];
    struct Obj *v104;        /* 0x104 */
    char pad108;
    u8 b109;                /* 0x109 */
    u8 b10A;                /* 0x10A */
    u8 b10B;                /* 0x10B */
} Obj;
typedef struct { Obj *who; s32 type; } Info;
typedef struct { char pad[0x10]; Info *info; } Ctx;
extern u32 D_8013960C;
extern u8 D_8013960A;
extern u16 D_8014767C;
extern s32 D_80147678;
extern u8 D_80156A09;
extern s32 func_80049CB4(s32, ...);
extern void func_800EBCD0(Obj *);
extern s32 func_800E4454(Obj *);
extern void func_800E4470(Obj *);
extern char *func_800EC7C8(Obj *);
extern char *func_800A3B20(Obj *);
extern void func_800498E4(s32, ...);
extern void func_80049BF0(s32);
extern Obj *func_800B4928(Vec *);
extern s32 func_800A5D2C(Obj *, Vec *, s32);
extern void func_800A59A4(Obj *);
extern s32 func_800A6184(Obj *, Vec *);
extern s32 func_800B48C0(Vec *, Obj *);
extern s32 func_800B5300(Obj *, Obj *, u8);
extern s32 func_800E1CC4(Obj *, s32);
extern s32 func_800E0F40(Obj *);
extern void func_800EDFF0(Obj *, s32);
void func_800EDB04(Obj *o, Ctx *ctx) {
    Info *info = ctx->info;
    char *found;
    if (info->type != 0x15) o->h28 = 0;
    func_80049CB4(0x1089, o);
    if (o->v104) {
        D_8013960C <<= 1;
        func_800EBCD0(o);
        D_8013960C >>= 1;
        func_80049CB4(0x93, o);
        func_80049CB4(0xDD);
    }
    if (func_800E4454(o)) func_800E4470(o);
    if (info->type != 10) func_80049CB4(0x70, o);
    found = func_800EC7C8(o);
    if (found == 0) D_8013960A = 0;
    if (info->type != 0x15) {
        D_8013960C = (D_8013960C << 1) | 1;
        func_800498E4(0x49, func_800A3B20(o));
        D_8013960C >>= 1;
    }
    func_80049CB4(0x129, 0xF);
    func_80049BF0(0);
    if (found) {
        Vec pos;
        Vec *pp = &pos;
        Obj *other;
        pp->x = o->pos.x;
        pp->y = o->pos.y;
        other = func_800B4928(pp);
        if (other != 0 && other != o) {
            s32 same = func_800A5D2C(o, pp, 10) == 1;
            if (!same) {
                func_800A59A4(other);
                func_800A6184(other, pp);
            }
            o->pos = pos;
        }
        func_800B48C0(&pos, o);
        o->b109 = info->type;
        o->v6C = 0;
        o->flags |= 0x40;
        if (info->type == 0x22) {
            func_800B5300(o, 0, D_80156A09);
            func_800498E4(0x107);
        }
        D_8014767C |= 0x40;
    } else {
        s32 low;
        func_80049CB4(2);
        o->h28 = 0;
        o->b109 = info->type;
        low = (u8)(o->b109 - 1) < 6;
        if (info->who != 0) {
            if (info->who == o) {
                if (low) o->b10A = 0x17;
                else o->b10A = 0;
                o->b10B = 1;
            } else {
                s32 hit;
                if (!low) o->b109 = 6;
                hit = 0;
                if (((info->who->b1E & 0x7C) && func_800E1CC4(info->who, 1)) || func_800E1CC4(o, 0)) hit = 1;
                if (hit) o->b109 = 0x29;
                o->b10A = info->who->bA;
                if (info->who->b1E & 0x7C) o->b10B = func_800E0F40(info->who);
                else o->b10B = 1;
            }
        } else {
            if (low) o->b109 = 0;
            o->b10A = 0;
            o->b10B = 1;
        }
        D_8013960C <<= 1;
        func_800EDFF0(o, 0);
        D_80147678 = 1;
        D_8013960C >>= 1;
    }
}
