#include "common.h"

/* Double-buffered display pools (see func_8006EDCC): func_8006E7E0 stores func_8006A8D8
 * allocations of 16-byte and 64-byte records in these two-entry pointer arrays. */
typedef struct { s32 word[4]; } PoolRecord16;
typedef struct { s32 word[16]; } PoolRecord64;

extern PoolRecord16 *D_801A7150[2];
extern PoolRecord64 *D_801A715C[2];
extern s32 D_8013D454;
void func_800713F8(void);
void func_8006E8A0(void) {
    D_801A7150[0] = 0;
    D_801A7150[1] = 0;
    D_801A715C[0] = 0;
    D_801A715C[1] = 0;
    func_800713F8();
    D_8013D454 = 0;
}
