#include "common.h"
typedef struct { s32 field_0; s32 field_4; unsigned char field_8; } Key80042260;
unsigned char *func_800B4D80(Key80042260 *key);
s32 func_80042260(s32 a0, s32 a1) {
    Key80042260 key;
    unsigned char *entry;
    key.field_4 = a0;
    key.field_0 = a1;
    entry = func_800B4D80(&key);
    if (entry == 0) return -1;
    if (entry[1] != 0xE7) return -1;
    key.field_8 = entry[0x10];
    return key.field_8;
}
