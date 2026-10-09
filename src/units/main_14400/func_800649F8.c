#include "common.h"
typedef struct { char pad00[0x18]; u32 data18; char pad1C[0x18]; } Entry;
extern Entry D_8013BAD4[];
extern Entry *D_8013B984;
extern u32 D_8016DC00;
extern float *D_8013B804;
extern void func_8006AAF0(void *, u32, s32);
extern void *func_80064A70(u32);
void func_800649F8(u32 *entryIndex)
{
    u32 count;
    u32 index;
    if (*entryIndex >= 28) {
        *entryIndex = 0;
    }
    index = *entryIndex;
    D_8016DC00 = index;
    D_8013B984 = &D_8013BAD4[index];
    func_8006AAF0(&count, D_8013B984->data18, 4);
    D_8013B804 = func_80064A70(0);
}
