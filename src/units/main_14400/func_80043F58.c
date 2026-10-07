#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

void func_80043D74(u32 deviceAddress, void *source, u32 length);

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    u8 pad10[0xC];
    u32 unk1C;
    u8 unk20[0x20];
    s32 unk40;
    s32 unk44;
} Obj80043F58;

void func_80043F58(Obj80043F58 *obj) {
    if (obj->unkC == 0) {
        obj->unk44 = 0;
        if (obj->unk40 >= 0) {
            func_80043D74(obj->unk1C + obj->unk40, obj->unk20, 0x20);
        }
    }
}
