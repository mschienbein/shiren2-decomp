#include "common.h"
/* Embedded 0x18-byte collection; its layout belongs to func_800CEC90. */
typedef struct { unsigned char storage[0x18]; } Collection800F838C;
typedef struct { unsigned char pad_0[0x8C]; Collection800F838C field_8C; } Obj;
typedef struct Obj_800EE358 Obj_800EE358;
typedef struct Obj800A38A0 Obj800A38A0;
extern void func_800CE6A0(Collection800F838C *obj, s32 flags);
extern void func_800E016C(Obj_800EE358 *obj, s32 flags);
extern void func_800A3918(Obj800A38A0 *obj);
void func_800F838C(Obj *obj, s32 flags) {
    func_800CE6A0(&obj->field_8C, 2);
    func_800E016C((Obj_800EE358 *)obj, 0);
    if (flags & 1) func_800A3918((Obj800A38A0 *)obj);
}
