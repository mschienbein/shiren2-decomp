#include "common.h"
typedef union { unsigned short index; unsigned char bytes[2]; } ItemCode;
extern const unsigned char D_80151A3C[64];
/* Command +0x1C returns a full-width signed encoded length. */
s32 func_800D9034(ItemCode *code, unsigned char *result) {
    *result = code->bytes[1];
    return D_80151A3C[code->index];
}
