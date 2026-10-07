#include "common.h"

/* Two 12-byte records; D_8013D90A is the first record's +6 field. */
typedef struct {
    short f00;
    short f02;
    short f04;
    short f06;
    short f08;
    short f0A;
} Record_8013D904;

extern Record_8013D904 D_8013D904[2];

s32 func_80077BA4(s32 kind, s32 value) {
    s32 index;

    switch (kind) {
    case 0x17:
        index = 0;
        break;
    case 0x1A:
        index = 1;
        break;
    default:
        return -1;
    }
    D_8013D904[index].f06 = value;
    return 0;
}
