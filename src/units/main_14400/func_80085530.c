#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct {
    u8 pad0[4];
    u16 field_4;
    u8 pad6[2];
    u16 field_8;
    u8 padA[4];
    u16 field_E;
    u8 pad10[4];
    s32 field_14;
    u8 pad18[4];
    s32 field_1C;
    u8 pad20[0x5C - 0x20];
    s32 field_5C;
    u8 pad60[8];
    s32 field_68;
} Obj;
s32 func_80084014(s32 a, s32 b);
struct Entry *func_8007946C(s32 a, s32 b);
void func_80079560(s32 a, s32 b, s32 c);
void func_80085530(Obj *o) {
    if (func_80084014(o->field_5C, o->field_68) == 0) {
        o->field_4 = 4;
        return;
    }
    func_8007946C(3, o->field_14);
    switch (o->field_8) {
    case 0:
        func_80079560(3, o->field_14, 1);
        o->field_E = 1;
        o->field_8++;
        break;
    case 1:
        if (o->field_1C-- == 0) {
            func_80079560(3, o->field_14, 0);
            o->field_4 = 4;
        }
        break;
    }
}
