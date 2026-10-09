#include "common.h"

typedef struct {
    unsigned char unk00[0x20];
    u32 unk20;
    unsigned char unk24[0x6C];
    u32 unk90;
} Object;
extern s32 func_800E0F40(Object *);

void func_80106444(Object *object) {
    if ((u32)(func_800E0F40(object) & 0xFF) >= 2) {
        object->unk90 |= 0x400000;
    } else {
        object->unk90 &= 0xFFBFFFFF;
    }
    object->unk20 = object->unk90;
}
