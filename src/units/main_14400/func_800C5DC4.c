#include "common.h"

extern u32 func_800413C0(void);
extern u32 func_800C5E8C(void *state, u32 seed);

void func_800C5DC4(void *arg0) {
    func_800C5E8C(arg0, func_800413C0());
}
