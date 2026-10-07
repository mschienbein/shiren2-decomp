#include "common.h"

typedef struct { char pad0[0x4]; s32 id; } Thread;

extern Thread *D_80037340;

s32 func_8002AA90(Thread *thread)
{
    if (thread == 0) {
        thread = D_80037340;
    }
    return thread->id;
}
