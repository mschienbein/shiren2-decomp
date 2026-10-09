#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_00[0x94]; u8 field_94; u8 pad_95[0x4F]; unsigned short field_E4; } Object;
extern s32 func_801169C4(void);
extern void func_800E9924(Object *object, s32 amount);
void func_800ECE3C(Object *object, s32 amount) {
    s32 factor = (u8)func_801169C4();
    if (factor >= 2) amount *= factor;
    func_800E9924(object, amount);
    if (((object->field_94 >> 2) & 1) && amount > 0) object->field_E4 |= 0x20;
}
