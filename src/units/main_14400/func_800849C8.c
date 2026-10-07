#include "common.h"
typedef struct { s32 field_0; unsigned char pad4[0x70]; } Entry800849C8;
extern Entry800849C8 D_801BA380[];
s32 func_800849C8(void) {
    s32 count = 0;
    s32 i;
    for (i = 0xAC; i != -1; i--) {
        if (D_801BA380[i].field_0 != 0) count++;
    }
    return count;
}
