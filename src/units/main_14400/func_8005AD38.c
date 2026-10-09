#include "common.h"
typedef struct { float x, y, z; } Vector;
extern Vector D_80165300;
extern unsigned char D_801658F4[0x40];
extern s32 D_80165404, D_80165394, D_80165400, D_801653F8;
void func_80059590(s32 id);
void func_800265E0(void *destination, s32 size);
void func_8005AD38(s32 mode)
{
    func_80059590(0);
    D_80165300.x = D_80165300.y = D_80165300.z = 0.0f;
    func_800265E0(D_801658F4, 0x40);
    D_80165404 = mode;
    D_80165394 = 3;
    D_80165400 = 0;
    D_801653F8 = 0;
}
