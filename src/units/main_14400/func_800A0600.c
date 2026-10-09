#include "common.h"
typedef struct { void *attacker_00; s32 kind_04; signed char percentage_08; } Request800A0600;
extern Request800A0600 D_80142904;
void func_800A0600(void)
{
    Request800A0600 *request = &D_80142904;
    request->attacker_00 = 0;
    request->kind_04 = 0;
    request->percentage_08 = 0;
}
