#include "common.h"

typedef unsigned char u8;

typedef struct { s32 x; s32 y; } Pos;
typedef struct { u8 pad0[4]; void *room; } Party;
typedef struct { u8 pad0[0x80]; Party *party; u8 pad84[0x1C]; s32 unkA0; s32 unkA4; } Obj;
typedef struct { u8 data[0x10]; } Spawn;
extern void *D_801476B8;
extern unsigned char D_80140160[];
char *func_800A3B20(Obj *obj);
s32 func_80049CB4(s32 id, ...);
void func_800498E4(s32 id, ...);
void func_80049BF0(s32 arg0);
s32 func_800E2074(Obj *obj);
void *func_800A65E4(Pos *out, Obj *obj, void *arg2);
void func_800A665C(Obj *obj, Pos *pos);
s32 func_800A31C8(void *room, Obj *obj);
s32 func_800D2C5C(Party *party);
s32 func_800D2FB0(Party *party);
s32 func_800EB820(void *arg0);
s32 func_800EB8B0(s32 arg0);
Spawn *func_800F68F4(Spawn *spawn, Party *party, char *name, s32 count);
Spawn *func_800F69E0(Spawn *spawn, Party *party, char *name, s32 count);
void *func_80093B58(void *arg0, Spawn *spawn);
s32 func_800D8FF0(void *item);
s32 func_800F66C4(Obj *obj, void *arg1) {
    Spawn spawn;
    Pos pos;
    Pos *p;
    char *name;
    s32 items;
    s32 gold;
    s32 bad;
    void *item;
    name = func_800A3B20(obj);
    func_80049CB4(0x128, 0x1A6);
    {
        s32 failed = func_800E2074(obj) != 1;
        p = &pos;
        if (failed) {
            func_800498E4(0x1BD, name);
            func_80049BF0(0);
            return 1;
        }
    }
    func_800A65E4(p, obj, arg1);
    func_800A665C(obj, p);
    func_80049CB4(2);
    if (obj->unkA0 != 0) {
        func_800498E4(0x1D2, name);
        func_80049BF0(0);
        return 1;
    }
    if (obj->unkA4 != 0) {
        func_800498E4(0x1D3, name);
        func_80049BF0(0);
        return 1;
    }
    bad = obj->party->room != 0 && func_800A31C8(obj->party->room, obj) == 0;
    if (bad) {
        func_800498E4(0x1D4, name);
        func_80049BF0(0);
        return 1;
    }
    items = func_800D2C5C(obj->party);
    gold = func_800D2FB0(obj->party);
    gold -= gold * func_800EB8B0(func_800EB820(D_801476B8)) / 100;
    if (items == 0) {
        if (gold == 0) {
            func_800498E4(0x1CC, name);
            func_80049BF0(0);
            return 1;
        }
    } else {
        func_800F68F4(&spawn, obj->party, name, items);
        item = func_80093B58(D_80140160, &spawn);
        if (item == 0 || (u32)(func_800D8FF0(item) - 4) < 2) {
            return 1;
        }
    }
    if (gold != 0) {
        s32 kind;
        func_800F69E0(&spawn, obj->party, name, gold);
        item = func_80093B58(D_80140160, &spawn);
        if (item == 0) {
            return 1;
        }
        kind = func_800D8FF0(item);
        if ((u32)(kind - 4) < 2) {
            return 1;
        }
    }
    return 1;
}
