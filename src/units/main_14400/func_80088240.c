#include "common.h"
typedef struct { char pad[4]; unsigned short field_4; char pad6[2]; unsigned short field_8; char padA[0xA]; s32 field_14; char pad18[4]; s32 field_1C; char pad20[0x10]; float field_30, field_34; } Obj;
typedef struct { char pad[0x14]; float field_14; char pad18[0x2E]; unsigned char field_46; char pad47[0x29]; unsigned char field_70[8]; } Effect;
extern Effect *func_8007946C(s32, s32);
extern void func_80079560(s32, s32, s32), func_8007935C(s32);
void func_80088240(Obj *a) {
    Effect *p = func_8007946C(4, a->field_14); unsigned char *color = p->field_70;
    switch (a->field_8) {
    case 0:
        color[0] = 0; color[1] = 0; color[2] = 0; color[3] = 0;
        color[4] = 0; color[5] = 0; color[6] = 0; color[7] = 255;
        a->field_30 = 0.5f; a->field_34 = 1.0f;
        func_80079560(4, a->field_14, 0);
        p->field_46 = 255;
        a->field_1C = 15;
        a->field_8++;
        break;
    case 1:
        if (a->field_1C--) {
            color[3] += 10; p->field_14 -= a->field_30; a->field_30 *= 1.5; p->field_46 -= 15;
        } else { func_8007935C(a->field_14); a->field_4 = 4; }
        break;
    }
}
