#include "common.h"

/* Double-buffered display pools allocated by func_8006E7E0 through func_8006A8D8:
 * D_801A7150[i] holds 0x5DC0 bytes of 16-byte records and D_801A715C[i] holds 0x6400
 * bytes of 64-byte records. D_801A7158/D_801A7164 are the cursors into the selected
 * buffer that func_800705F4/func_80070660 advance by those strides and return. */
typedef struct { s32 word[4]; } PoolRecord16;
typedef struct { s32 word[16]; } PoolRecord64;

extern s32 D_8013D454;
extern s32 D_8013D450;
extern PoolRecord16 *D_801A7150[2];
extern PoolRecord16 *D_801A7158;
extern PoolRecord64 *D_801A715C[2];
extern PoolRecord64 *D_801A7164;
void func_80071B80(void);

void func_8006EDCC(void) {
    if (D_8013D454 != 0) {
        D_801A7158 = D_801A7150[D_8013D450];
        D_801A7164 = D_801A715C[D_8013D450];
        func_80071B80();
    }
}
