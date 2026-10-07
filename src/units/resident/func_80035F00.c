#include "common.h"

typedef unsigned short u16;
typedef unsigned long long u64;
typedef s32 OSPri;
typedef void *OSMesg;
typedef u64 OSTime;
typedef struct OSMesgQueue OSMesgQueue;

typedef struct OSThread {
    struct OSThread *next;
    OSPri priority;
    struct OSThread **queue;
    struct OSThread *tlnext;
    u16 state;
    u16 flags;
} OSThread;

extern OSThread *D_80037338; /* __osRunQueue */
extern OSThread *D_80037340; /* __osRunningThread */

u32 func_8002AF70(void);
void func_8002AFE0(u32);

void func_8002A68C(OSThread **);

void func_80035F00(void) {
    register u32 saveMask = func_8002AF70();

    D_80037340->state = 2;
    func_8002A68C(&D_80037338);
    func_8002AFE0(saveMask);
}
