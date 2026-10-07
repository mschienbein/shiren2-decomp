#include "common.h"

typedef unsigned long long u64;
typedef u64 OSTime;

extern u32 func_8002AF70(void);
extern u32 func_8002A9B0(void);
extern void func_8002AFE0(u32 mask);
extern u32 D_80039000;
extern OSTime D_80039010;

/* osGetTime */
OSTime func_8002AAB0(void)
{
    u32 tmptime;
    u32 elapseCount;
    OSTime currentCount;
    register u32 saveMask;

    saveMask = func_8002AF70();
    tmptime = func_8002A9B0();
    elapseCount = tmptime - D_80039000;
    currentCount = D_80039010;
    func_8002AFE0(saveMask);
    return currentCount + elapseCount;
}
