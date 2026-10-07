#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { u8 value; } Dir;
typedef struct { s32 type; void *sender; s32 data[6]; } Event;
/* Item vtable slot +0x38/+0x3C: message handler; the type-3 target func_80110258 returns int. */
typedef struct { char pad[0x38]; s16 offset38; char pad3A[2]; s32 (*func3C)(void *, Event *); } VTable;
typedef struct { s32 unk0; s32 unk4; VTable *vtbl8; } Target;
typedef struct {
    char pad0[8];
    u8 dir8;
    char pad9[0x4F];
    void *unk58;
    char pad5C[0x2C];
    s32 unk88;
    char pad8C[0x50];
    u8 modeDC;
} Obj;
Target *func_800E8A68(Obj *, u8);
s32 func_800E20CC(Obj *);
void *func_800A6CC0(void *, void *);
void *func_800B4928(Pos *);
void *func_800A65E4(void *, void *, void *);
Pos func_8010ECD0(Target *, Obj *, Dir *);
s32 func_800A251C(Pos *, void *);
u8 func_800A6420(Obj *, void *);
s32 func_800E1CC4(Obj *, s32);
void func_800A665C(Obj *, Dir *);
void func_800E8FAC(Obj *);
s32 func_8010A454(Obj *obj) {
    Target *target = func_800E8A68(obj, 3);
    Pos pos;
    Dir dir;
    dir.value = obj->dir8;
    if (func_800E20CC(obj)) {
        func_800A6CC0(&pos, obj);
        func_800B4928(&pos);
    } else {
        void *other;
        Dir direction;
        if (obj->unk88 == 0) {
            return 0;
        }
        other = obj->unk58;
        if (other == 0) {
            return 0;
        }
        func_800A65E4(&direction, obj, other);
        dir = direction;
        if (target != 0 && obj->modeDC == 2) {
            s32 blocked;
            pos = func_8010ECD0(target, obj, &dir);
            blocked = func_800A251C(&pos, other) ^ 1;
            if (blocked) {
                return 0;
            }
        }
        if (obj->modeDC == 0 && func_800A6420(obj, other)) {
            return 0;
        }
    }
    if (func_800E1CC4(obj, 4)) {
        dir.value = (dir.value + 4) & 7;
    }
    func_800A665C(obj, &dir);
    if (target == 0) {
        func_800E8FAC(obj);
    } else {
        Event event;
        event.type = 0x10;
        event.sender = obj;
        (void)target->vtbl8->func3C((char *)target + target->vtbl8->offset38, &event);
    }
    return 1;
}
