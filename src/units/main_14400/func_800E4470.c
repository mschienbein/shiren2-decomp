#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { char pad[0xA]; u8 unkA; char padB[0x14]; u8 unk1F; char pad20[0x55]; u8 unk75; } Obj;
extern u16 D_8014767C;
s32 func_800E4454(Obj *);
s32 func_800E0F40(Obj *);
void func_800E43EC(Obj *, u16, u8);
void func_800E4470(Obj *obj) {
    if (func_800E4454(obj)) {
        if (D_8014767C & 3) {
            s32 value = obj->unkA;
            func_800E43EC(obj, value, func_800E0F40(obj));
        } else {
            obj->unk1F = obj->unkA;
            obj->unk75 = func_800E0F40(obj);
        }
    }
}
