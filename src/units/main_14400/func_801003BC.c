#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { s32 x; s32 y; } Pos801003BC;

typedef struct {
    u8 pad0[0x58];
    Pos801003BC *target_58;
    u8 pad5C[0x3E];
    u16 flags_9A;
} Obj801003BC;

s32 func_800A692C(Obj801003BC *obj, s32 kind);
s32 func_800F3310(Obj801003BC *obj);
s32 func_800E0F40(Obj801003BC *obj);
u8 func_800A6420(Obj801003BC *obj, Pos801003BC *target);
s32 func_800A67DC(Obj801003BC *obj, Pos801003BC *target, s32 a2, s32 a3);
s32 func_800F1024(Obj801003BC *obj);
void func_800F06E4(Obj801003BC *obj);
s32 func_800E1CD4(Obj801003BC *obj, s32 flag);
s32 func_800E8350(Obj801003BC *obj);
s32 func_800E7AA8(Obj801003BC *obj, s32 a1);
s32 func_800E7104(Obj801003BC *obj);

s32 func_801003BC(Obj801003BC *obj) {
    Pos801003BC *target;
    u8 kind;

    if (func_800A692C(obj, 0x12) != 0) {
        return func_800F3310(obj);
    }
    target = obj->target_58;
    kind = (u8)func_800E0F40(obj);
    switch (func_800A6420(obj, target)) {
    case 0:
        if (kind != 2) {
            func_800F06E4(obj);
            return 0;
        }
        break;
    case 1:
    case 2:
        if (kind < 3) {
            s32 flag = obj->flags_9A & 0x40;
            s32 miss = func_800A67DC(obj, target, 0, flag != 0) != 1;

            if (miss) {
                break;
            }
        }
        if (func_800F1024(obj) == 0) {
            break;
        }
        func_800F06E4(obj);
        return 0;
    }
    if (func_800E1CD4(obj, 0x10) != 0) {
        return func_800E8350(obj);
    }
    if (kind == 2) {
        return func_800E7AA8(obj, 3);
    }
    return func_800E7104(obj);
}
