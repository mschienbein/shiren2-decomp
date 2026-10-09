#include "common.h"

typedef unsigned char u8;
typedef unsigned long long OSTime;
typedef union { OSTime ticks; struct { u32 high, low; } words; } Time;
typedef struct { Time start, end; } Timing;
typedef struct { u8 header[0x10]; u32 counts[8]; Timing slots[8][4]; } Snapshot;
/* Complete 0xA00-byte scheduler storage; only its refresh-rate byte is used, the rest is opaque. */
typedef struct { u8 pad000[0x9F0]; s32 retraces; u8 refresh; u8 rest[11]; } Scheduler;
extern Scheduler D_801D85D0;
extern Snapshot *D_801A70DC;
extern u32 func_80031F90(u32 mask);
extern OSTime func_8002AAB0(void);
extern OSTime func_80036510(OSTime numerator, OSTime denominator);

/* Every caller (C and assembly) passes constant ids 0..7 and on/off flags 0/1;
 * the callee narrows both on first use, so the parameters are u8. */
s32 func_8006D184(u8 id, u8 on) {
    s32 percentage = 0;
    u32 previous = func_80031F90(1);
    if (id < 8) {
        u32 *count = &D_801A70DC->counts[id];
        Timing *slot = &D_801A70DC->slots[id][*count];
        if (*count < 4) {
            if (on == 0) {
                slot->start.ticks = func_80036510(func_8002AAB0() << 6, 3000);
            } else {
                s32 period;
                u32 elapsed;
                slot->end.ticks = func_80036510(func_8002AAB0() << 6, 3000);
                period = 1000000 / D_801D85D0.refresh;
                elapsed = slot->end.words.low - slot->start.words.low;
                (*count)++;
                percentage = elapsed * 100 / period;
            }
        }
        func_80031F90(previous);
    }
    return percentage;
}
