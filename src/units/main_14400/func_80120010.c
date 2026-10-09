#include "common.h"

typedef unsigned short u16;
typedef short s16;
typedef struct Object Object;
extern s16 D_80156A2A;
extern s32 func_800EB350(Object *object);
extern void func_80049A04(u16 id, ...);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800EB598(void *obj, s16 a1, s16 a2);

/* Item vtable slot +0x44 of D_8015F618 (called as func44(self, entity) by func_8010E35C);
 * self is supplied but unused. */
void func_80120010(void *self, Object *target)
{
    func_800EB350(target);
    func_80049A04(0x14);
    func_80049CB4(0x12A);
    func_800EB598(target, 0, D_80156A2A);
}
