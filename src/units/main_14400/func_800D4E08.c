#include "common.h"

/* Pool container object (pool-record pointer +0, method table +4, mode byte +8); only its address is used here. */
typedef struct Pool Pool;
extern Pool D_80147F90;
extern void func_800D4C8C(Pool *, s32);

void func_800D4E08(void) {
    func_800D4C8C(&D_80147F90, 2);
}
