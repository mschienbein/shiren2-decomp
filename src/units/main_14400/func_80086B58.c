#include "common.h"

typedef struct {
    s32 unk00;
    unsigned short unk04;
    unsigned short unk06;
    unsigned short unk08;
    unsigned char unk0A[0xA];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    unsigned char unk20[0xC];
    s32 unk2C;
} Object;
typedef struct {
    unsigned char unk00[9];
    unsigned char unk09;
    unsigned char unk0A[0x12];
    float unk1C;
    float unk20;
    unsigned char unk24[0x1A];
    unsigned char unk3E;
    unsigned char unk3F;
    unsigned char unk40;
} Effect;
extern Effect *func_8007946C(s32, s32);
extern void func_80079560(s32, s32, s32);
extern void func_8007935C(s32);
extern s32 func_800751B4(s32, s32);

void func_80086B58(Object *object) {
    Effect *effect = func_8007946C(0, object->unk14);
    float scale;
    switch (object->unk08) {
    case 0:
        effect->unk09 = 0;
        effect->unk40 = 1;
        effect->unk3E = 0;
        func_800751B4(object->unk14, 0x8000);
        func_8007946C(4, object->unk2C);
        func_80079560(4, object->unk2C, 0);
        object->unk1C = 12;
        object->unk08++;
        break;
    case 1:
        if (object->unk1C-- != 0) {
            scale = effect->unk1C * 0.9f;
            effect->unk20 = scale;
            effect->unk1C = scale;
        } else {
            func_80079560(0, object->unk14, 1);
            func_8007935C(object->unk2C);
            object->unk04 = 4;
        }
        break;
    }
}
