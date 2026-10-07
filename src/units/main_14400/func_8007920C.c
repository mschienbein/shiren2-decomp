#include "common.h"
typedef float f32;
typedef struct { unsigned short id; unsigned short pad; } Code8007920C;
typedef struct {
    short field_0;
    unsigned char pad2[5];
    unsigned char field_7;
    unsigned char pad8[4];
    short field_C;
    short field_E;
    short field_10;
    unsigned char pad12[0xA];
    f32 field_1C;
    f32 field_20;
    f32 field_24;
    unsigned char pad28[0xD];
    unsigned char field_35;
    unsigned char pad36[2];
    f32 field_38;
    unsigned char pad3C[2];
    unsigned char field_3E;
    unsigned char pad3F;
    unsigned char field_40;
    unsigned char pad41[3];
    unsigned char field_44;
    unsigned char field_45;
    unsigned char pad46[0xB0 - 0x46];
} Slot8007920C;
extern Code8007920C D_8013F1F0[];
extern Code8007920C D_8013F360[];
/* Resident descriptors are opaque here; only their pointer presence is tested. */
typedef struct SpriteDesc8007920C SpriteDesc8007920C;
extern SpriteDesc8007920C *D_801D25A0[];
extern Slot8007920C D_801DD378[];
s32 func_80074500(Slot8007920C *slots, s32 a, s32 count, s32 id, s32 code);
s32 func_8007920C(s32 kind, s32 id, s32 x, s32 y) {
    s32 code;
    s32 index;
    Slot8007920C *slot;
    switch (kind) {
    case 0:
        code = id;
        break;
    case 0x10:
        code = D_8013F360[id - 0xD0].id;
        break;
    default:
        code = D_8013F1F0[kind].id;
        break;
    }
    index = func_80074500(D_801DD378, 0, 0x20, id, code);
    if (index == -1) return -1;
    slot = &D_801DD378[index];
    if (D_801D25A0[code] == 0) slot->field_44 = 0xFE;
    else slot->field_44 = 0xFF;
    slot->field_7 = 1;
    slot->field_0 = 4;
    slot->field_C = (x << 7) + 0x40;
    slot->field_10 = (y << 7) + 0x40;
    slot->field_E = 0;
    slot->field_35 = 1;
    slot->field_45 = 0;
    slot->field_3E = 0;
    slot->field_40 = 0;
    slot->field_24 = 1.0f;
    slot->field_20 = 1.0f;
    slot->field_1C = 1.0f;
    slot->field_38 = 0.75f;
    return index;
}
