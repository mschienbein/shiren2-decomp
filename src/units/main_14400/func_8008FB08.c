#include "common.h"
typedef unsigned char u8;
typedef struct { s32 fields_0[2]; u32 flags_8; s32 fields_C[2]; u8 data_14[24]; } Output;
/* Resource dispatch supplies an owner pointer; this apply callback does not use it. */
s32 func_8008FB08(const u8 *object, void *unused_owner, Output *output) {
    const u8 *source = object + 0x18;
    u8 *data;
    u32 flags = output->flags_8 | 4;
    output->flags_8 = flags;
    data = output->data_14;
    if (source[8] == 1) output->flags_8 = flags | 0x200000;
    output->data_14[0] = object[0x18];
    data[1] = source[1];
    data[2] = source[2];
    data[3] = source[3];
    data[4] = source[4];
    data[5] = source[5];
    data[6] = source[6];
    data[7] = source[7];
    data[8] = source[9];
    data[9] = source[10];
    data[10] = source[11];
    data[11] = source[12];
    data[12] = source[13];
    data[13] = source[14];
    data[14] = source[15];
    data[15] = source[16];
    data[16] = source[17];
    data[17] = source[18];
    data[18] = source[19];
    data[19] = source[20];
    data[20] = source[21];
    data[21] = source[22];
    data[22] = source[23];
    data[23] = source[24];
    return 0;
}
