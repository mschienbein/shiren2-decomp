#include "common.h"

typedef struct Obj_800F3E68 Obj_800F3E68;

extern s32 func_80049CB4(s32 id, ...);

void func_800A7C1C(Obj_800F3E68 *obj)
{
    func_80049CB4(0x88, obj);
    func_80049CB4(0x9D, obj);
}
