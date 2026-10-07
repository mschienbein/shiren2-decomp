#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { s32 x; s32 y; } Pos800E3884;
typedef struct Obj800E3884 Obj800E3884;
typedef struct { u8 pad0[0x90]; s16 offset_90; s16 pad92; s32 (*fn_94)(void *self, s32 a, s32 b, u8 c, s32 d); } VTable800E3884;
struct Obj800E3884 {
    Pos800E3884 pos;
    u8 pad8[0x14];
    u8 flags_1C;
    u8 pad1D;
    u8 flags_1E;
    u8 pad1F[5];
    VTable800E3884 *vtable_24;
    u8 pad28[0x2C];
    u8 flags_54;
    u8 pad55;
    u8 field_56;
};
typedef struct { Obj800E3884 *attacker; s32 kind; u8 pad8[6]; u16 flags_E; } Hit800E3884;
extern u8 D_801531A0[];
extern u16 D_80156A32;
extern u32 D_8013960C;
u16 func_800E08B0(Obj800E3884 *obj);
u16 func_800E08F0(Obj800E3884 *obj);
void func_800E20F0(Obj800E3884 *obj);
char *func_800A3B20(Obj800E3884 *obj);
s32 func_800E1CD4(Obj800E3884 *obj, s32 kind);
s32 func_800E1DA0(Obj800E3884 *obj);
s32 func_800A692C(Obj800E3884 *obj, s32 kind);
s32 func_800A6FD0(Obj800E3884 *obj);
s32 func_80049CB4(s32 id, ...);
void func_800497F0(s32 id, ...);
void func_800E44EC(Obj800E3884 *obj);
s32 func_800E0534(Obj800E3884 *obj, s32 amount);
void func_800A7B68(Obj800E3884 *obj, Hit800E3884 *hit);
void func_800E3678(Obj800E3884 *obj, Obj800E3884 *attacker);

void func_800E3884(Obj800E3884 *obj, Hit800E3884 *hit, s32 amount) {
    Pos800E3884 pos;
    Obj800E3884 *attacker;
    char *name;
    s32 flag;
    s32 msg;

    if (func_800E08B0(obj) == 0) {
        return;
    }
    attacker = hit->attacker;
    pos.x = obj->pos.x;
    pos.y = obj->pos.y;
    func_800E20F0(obj);
    if (hit->kind == 0x22) {
        obj->flags_54 |= 1;
    }
    if (amount == 0) {
        amount = 1;
    }
    name = func_800A3B20(obj);
    if (amount >= 0) {
        if (func_800E1CD4(obj, 0xC)) {
            obj->vtable_24->fn_94((u8 *)obj + obj->vtable_24->offset_90, 1, 0xC, 0, 0);
        }
        flag = 0;
        if (func_800E1CD4(obj, 0xF)) {
            flag = func_800E1DA0(obj) == 0;
        }
        if (flag) {
            obj->vtable_24->fn_94((u8 *)obj + obj->vtable_24->offset_90, 1, 0xF, 0, 0);
            obj->field_56 = 0;
            return;
        }
        {
            s32 boosted = 0;

            if (!(D_801531A0[hit->kind] & 1)) {
                boosted = func_800A692C(obj, 5) != 0;
            }
            if (boosted) {
                amount = D_80156A32;
            }
        }
    }

    flag = 0;
    if ((attacker != 0 && func_800A6FD0(attacker)) || func_800A6FD0(obj)) {
        flag = 1;
    }
    msg = -1;
    if (!flag) {
        msg = func_80049CB4(0xDA, &pos);
    }
    if (msg != -2) {
        if (amount >= 0) {
            s32 kind;

            if (!(D_801531A0[hit->kind] & 2)) {
                s32 id = 0x1021;

                if (msg == -1) {
                    if (func_80049CB4(0xDA, &pos) != -2) {
                        id = 0x1021;
                    } else {
                        id = 0x1022;
                    }
                }
                func_80049CB4(6);
                func_80049CB4(id, obj, amount >= func_800E08B0(obj), hit->flags_E);
                func_80049CB4(7);
            }
            kind = hit->kind;
            if (!(D_801531A0[kind] & 4)) {
                flag = 0;
                if (kind == 1) {
                    if (func_800A6FD0(obj)) {
                        func_800497F0(0x40, msg, func_800A3B20(attacker), amount);
                    } else {
                        char *attackerName = func_800A3B20(attacker);

                        func_800497F0(0x3C, msg, attackerName, func_800A3B20(obj), amount);
                    }
                } else {
                    if (func_800A6FD0(obj) || (obj->flags_1E & 0xC)) {
                        flag = 1;
                    }
                    if (flag || attacker == 0 || hit->kind == 0x22 || attacker == obj) {
                        if (obj->flags_1C & 1) {
                            func_800497F0(0x41, msg, name, amount);
                        } else {
                            func_800497F0(0x42, msg, amount);
                        }
                    } else {
                        if (obj->flags_1C & 1) {
                            func_800497F0(0x3D, msg, name, amount);
                        } else {
                            func_800497F0(0x3E, msg, amount);
                        }
                    }
                }
            }
            if ((hit->flags_E >> 9) & 1) {
                func_800E44EC(obj);
            }
        } else {
            s32 kind = hit->kind;

            if (!(D_801531A0[kind] & 4)) {
                s32 heal = -amount;

                if (kind != 0x21) {
                    s32 room = func_800E08F0(obj) - func_800E08B0(obj);

                    if (room < heal) {
                        heal = room;
                    }
                }
                if (heal != 0) {
                    func_80049CB4(0x128, 0x6D);
                    if (obj->flags_1E & 0xC) {
                        func_800497F0(2, msg, func_800A3B20(obj), heal);
                    } else {
                        func_800497F0(3, msg, func_800A3B20(obj), heal);
                    }
                }
            }
        }
    }
    D_8013960C <<= 1;
    func_800E0534(obj, -amount);
    D_8013960C >>= 1;
    if (func_800E08B0(obj) == 0) {
        func_800A7B68(obj, hit);
        return;
    }
    if (!(D_801531A0[hit->kind] & 0x42)) {
        func_800E3678(obj, attacker);
    }
}
