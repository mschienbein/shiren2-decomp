#include "common.h"

typedef struct Obj_80049414 Obj_80049414;
extern unsigned short D_801569DE;
extern s32 func_800A99D0(void);
extern void func_800498E4(s32 id, ...);
extern s32 func_800E1CC4(Obj_80049414 *object, s32 kind);
extern s32 func_800E07A8(void *object, s32 amount);
extern s32 func_80049CB4(s32 id, ...);

/* Item vtable slot +0x44: `action` is the adjusted receiver the dispatcher supplies; not used here. */
void func_801183F0(void *action, Obj_80049414 *target)
{
    if (func_800A99D0()) {
        func_800498E4(0x222);
    } else {
        s32 multiplier = func_800E1CC4(target, 3) ? 2 : 1;
        if ((short)func_800E07A8(target, D_801569DE * multiplier) > 0)
            func_80049CB4(0x128, 0x73);
    }
}
