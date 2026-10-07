#include "common.h"

typedef struct { char pad0[0x14]; s32 priority; } Thread;

extern Thread *D_80037340;

s32 func_8002AA70(Thread *thread)
{
    if (thread == 0) {
        thread = D_80037340;
    }
    return thread->priority;
}
