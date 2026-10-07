#include "common.h"

/* Element stride 0x74 (116 bytes); contents unknown. */
typedef struct {
    unsigned char bytes[0x74];
} Entry_80084AB4;

extern Entry_80084AB4 D_801BA380[];

Entry_80084AB4 *func_80084AB4(s32 index)
{
    return &D_801BA380[index];
}
