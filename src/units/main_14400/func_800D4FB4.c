#include "common.h"

typedef unsigned char u8;
typedef struct { char pad[8]; u8 unk8; char pad9[3]; void *unkC; } Obj;
/* Returns D_8015483C[index]: the address of D_80147FA0 or D_80147FAC, else NULL. */
void *func_800D4EA0(u32 index);
void func_800D4FB4(Obj *obj, u32 index) {
    obj->unk8 = index;
    if (index < 2) {
        obj->unkC = func_800D4EA0(index);
    }
}
