#include "common.h"

typedef struct {
    unsigned char unk00[0xC];
    unsigned char unk0C;
} Object;
extern unsigned char func_800AE98C(Object *);
extern s32 func_800AC584(unsigned short);
extern unsigned short D_80157698[];
extern unsigned char D_801576FC[];

u32 func_801118AC(Object *object) {
    s32 index = func_800AE98C(object) & 0xFF;
    u32 value = func_800AC584(D_80157698[index]);
    return value + (value * D_801576FC[index] * object->unk0C) / 100;
}
