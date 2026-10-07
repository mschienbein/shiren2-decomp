#include "common.h"

/* Two observed pairs of 32-bit words in main BSS; meaning unknown. */
typedef struct {
    u32 word0;
    u32 word4;
} Pair_800535A4;

extern Pair_800535A4 D_80161700;
extern Pair_800535A4 D_80161708;

void func_800535A4(void)
{
    D_80161700.word0 = 0;
    D_80161700.word4 = 0;
    D_80161708.word0 = 0;
    D_80161708.word4 = 0;
}
