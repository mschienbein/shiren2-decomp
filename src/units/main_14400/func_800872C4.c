#include "common.h"

typedef struct {
    s32 unk00;
    unsigned short unk04;
    unsigned short unk06;
    unsigned short unk08;
    unsigned char unk0A[0xA];
    s32 unk14;
    unsigned char unk18[0x1C];
    float unk34;
    unsigned char unk38[0x10];
    float unk48;
    float unk4C;
} Object;
typedef struct { unsigned char unk00[0x14]; float unk14; } Effect;
extern Effect *func_8007946C(s32, s32);
extern void func_80079560(s32, s32, s32);
extern void func_8007935C(s32);

void func_800872C4(Object *object) {
    Effect *effect = func_8007946C(4, object->unk14);
    switch (object->unk08) {
    case 0:
        func_80079560(4, object->unk14, 0);
        object->unk34 = 200.0f;
        object->unk48 = 20.0f;
        object->unk4C = effect->unk14;
        object->unk08++;
        /* Continue with the first animation step. */
    case 1:
        object->unk34 -= object->unk48;
        object->unk4C -= object->unk48;
        object->unk48 *= 1.08f;
        if (object->unk34 >= 0.0f) {
            effect->unk14 = object->unk4C;
        } else {
            func_8007935C(object->unk14);
            object->unk04 = 4;
        }
        break;
    }
}
