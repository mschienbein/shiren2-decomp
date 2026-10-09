#include "common.h"
typedef struct Unit80096140 {
    unsigned char pad_00[0xA];
    unsigned char field_0A;
    unsigned char pad_0B[0x14];
    unsigned char field_1F;
} Unit80096140;
s32 func_800E4454(Unit80096140 *unit) {
    unsigned int other = unit->field_1F;
    return unit->field_0A != other && other < 0xD5;
}
