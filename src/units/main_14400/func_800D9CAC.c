#include "common.h"

typedef struct Effect Effect;
typedef struct S S;
typedef struct Object { unsigned char pad_00[8]; Effect *effect_08; } Object;
extern S *D_801476B8;
extern s32 func_800D2C5C(Effect *effect);
extern void func_800D2D94(Effect *effect);
extern void func_800EB744(S *object, s32 delta);
extern s32 func_80049CB4(s32 id, ...);

s32 func_800D9CAC(Object *object)
{
    s32 amount = func_800D2C5C(object->effect_08);
    func_800D2D94(object->effect_08);
    func_800EB744(D_801476B8, amount);
    func_80049CB4(0x89, D_801476B8);
    func_80049CB4(2);
    return 1;
}
