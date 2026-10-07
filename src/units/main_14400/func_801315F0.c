#include "common.h"
extern void (*D_80148D90)(s32);
u32 func_80031F90(u32);
void func_801315F0(void (*callback)(s32)) {
    u32 saved = func_80031F90(1);
    D_80148D90 = callback;
    func_80031F90(saved);
}
