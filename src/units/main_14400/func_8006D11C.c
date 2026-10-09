#include "common.h"

typedef union { u32 alignment; unsigned char bytes[0x230]; } Snapshot;
extern Snapshot *D_801A70D8;
extern u32 func_80031F90(u32 mask);

void func_8006D11C(Snapshot *out)
{
    u32 previous = func_80031F90(1);
    *out = *D_801A70D8;
    func_80031F90(previous);
}
