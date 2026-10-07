#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef float f32;

typedef struct {
    u16 scale;
    u8 size;
    u8 pad3[3];
    u16 anim;
} ActorData;
typedef struct {
    ActorData *data;
    u8 pad4;
    u8 palette;
} ActorInfo;
typedef struct {
    s16 unk0;
    u8 pad2[4];
    u8 variant;
    u8 pad7[5];
    s16 x;
    s16 unkE;
    s16 z;
    u8 pad12[0xA];
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;
    u8 pad28[0xD];
    u8 active;
    u8 unk36;
    u8 pad37;
    f32 scale;
    u8 pad3C[8];
    u8 unk44;
    u8 index;
    u8 pad46[4];
    s16 unk4A;
    u8 pad4C[0x64];
} Actor;
typedef struct {
    s16 unk0;
    s16 unk2;
    u8 pad4[0xAC];
} Shadow;
typedef struct {
    s32 id;
    s32 value;
} Slot;
typedef struct {
    Slot slots[3];
} SlotSet;
extern Actor D_801DEAB4[];
extern Shadow D_801D2C2C[];
extern SlotSet D_801A7320[];
ActorInfo *func_80074784(s32, s32);
s32 func_80042734(s32);
s32 func_80074500(Actor *, s32, s32, s32, s32);
s16 func_80076C34(s32, s32, s32);
void func_8007482C(s32, s32);
s32 func_80076044(s32, s32, s32, s32, s32);
void func_80075E5C(s32, s32);
void func_80074778(Shadow *);
s32 func_800748F8(s32 owner, s32 type, s32 x, s32 z, s32 arg4, s32 variant) {
    ActorInfo *info = func_80074784(type, variant);
    s32 anim;
    s32 index;
    Actor *actor;
    SlotSet *set;
    Shadow *shadow;
    s32 none;

    if (type == 0x47 && func_80042734(owner) == 2) {
        anim = 0x178;
    } else {
        anim = info->data->anim;
    }
    index = func_80074500(D_801DEAB4, owner, 0, type, anim);
    if (index == -1) {
        return -1;
    }
    actor = &D_801DEAB4[index];
    actor->x = (x << 7) + 0x40;
    actor->unk0 = 0;
    actor->variant = variant;
    actor->active = 1;
    actor->z = (z << 7) + 0x40;
    actor->unk4A = arg4;
    actor->unk44 = 1;
    actor->index = index;
    actor->scaleX = info->data->size / 100.0f;
    actor->scaleY = info->data->size / 100.0f;
    actor->scaleZ = info->data->size / 100.0f;
    actor->scale = info->data->scale / 100.0f;
    actor->unkE = func_80076C34(index, 2, 0);
    switch (type) {
    case 0x1C:
    case 0x49:
    case 0x53:
    case 0x56:
        actor->unk36 = 4;
        break;
    case 0x1E:
        actor->unk36 = 4;
        break;
    }
    if (info->palette != 0) {
        func_8007482C(index, info->palette);
    } else {
        func_8007482C(index, variant);
    }
    none = -1;
    func_80076044(index, anim, 1, 8, 3);
    func_80075E5C(index, 0);
    {
        Shadow *shadows = D_801D2C2C;

        shadow = &shadows[index];
    }
    if (shadow->unk2 != none) {
        func_80074778(shadow);
    }
    set = &D_801A7320[index];
    set->slots[0].id = none;
    set->slots[1].id = none;
    set->slots[2].id = none;
    set->slots[0].value = 0;
    set->slots[1].value = 0;
    set->slots[2].value = 0;
    return index;
}
