#include "common.h"

typedef struct { unsigned char pad_00[0x1E]; unsigned char field_1E; } Obj;
typedef Obj Obj_80049414;
typedef Obj Obj_800EB488;
typedef short s16;
extern unsigned short D_801569E0;
extern s32 func_800A99D0(void);
extern void func_800498E4(s32 id, ...);
extern s32 func_800E1CC4(Obj_80049414 *obj, s32 kind);
extern void func_800EB488(Obj_800EB488 *obj, s16 amount);
/* Item vtable slot +0x44: `self` is the adjusted receiver the dispatcher supplies; not used here. */
void func_80118500(void *self, Obj *obj)
{
    if (func_800A99D0()) {
        func_800498E4(0x222);
    } else if ((obj->field_1E >> 2) & 1) {
        s32 multiplier = func_800E1CC4(obj, 3) ? 2 : 1;
        func_800EB488(obj, (s16)(D_801569E0 * multiplier));
    }
}
