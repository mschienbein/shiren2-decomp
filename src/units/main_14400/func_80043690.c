#include "common.h"

/* Two 0x50-byte slots; the bitmap has one bit per slot. */
typedef struct { unsigned char data[0x50]; } Slot;
extern Slot D_801609D0[2];
extern unsigned char D_80138B04;

/* The caller-supplied allocation size is unused for this fixed-size pool. */
void *func_80043690(s32 size)
{
    s32 i = 2;
    s32 mask;

    /* ODD_C: the do/while (0) groups the search so that exhausting the
     * pool breaks out to the shared failure return (the break is taken
     * once every slot is in use); its loop notes also make reorg predict
     * that exit and fill its delay slot with the null result.  The retry
     * edge expresses the descending search without GCC's counted-loop
     * strength reduction of the slot address. */
    /* ODD_C: measured: without the block (break -> return 0) 20 words differ, 92 vs 96 bytes;
     * as a plain descending for loop 19 words differ, 76 bytes. */
    do {
retry:
        --i;
        if (i == -1) {
            break;
        }
        mask = 1 << i;
        if (D_80138B04 & mask) {
            goto retry;
        }
        D_80138B04 |= mask;
        return &D_801609D0[i];
    } while (0);
    return 0;
}
