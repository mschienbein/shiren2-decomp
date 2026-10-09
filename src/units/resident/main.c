#include "common.h"

typedef unsigned long long u64;
typedef struct OSThread OSThread;
extern OSThread D_80039368;
/* The main-thread stack begins immediately after D_80039368. */
extern u64 D_80039518[0x4000 / sizeof(u64)];
extern void func_8002AC88(void);
extern void func_80027ED0(OSThread *, s32, void (*)(void *), void *, void *, s32);
extern void func_80032B50(OSThread *);
extern void func_80025CC8(void *);

void main(void *argument)
{
    func_8002AC88();
    func_80027ED0(&D_80039368, 1, func_80025CC8, argument, &D_80039518[0x4000 / sizeof(u64)], 50);
    func_80032B50(&D_80039368);
}
