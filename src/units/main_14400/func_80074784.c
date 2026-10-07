#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 data[8];
} Entry_80074784;

extern Entry_80074784 D_8013F3D0[];
extern Entry_80074784 D_8013FE90[];
extern Entry_80074784 D_8013FEB0[];
extern Entry_80074784 D_8013FEC8[];

Entry_80074784 *func_80074784(s32 kind, s32 level) {
    s32 index = 0;

    if (level > 0) {
        index = level - 1;
    }
    switch (kind) {
        case 0x37:
            if (index >= 4) {
                index = 3;
            }
            return &D_8013FE90[index];
        case 0x31:
            if (index >= 3) {
                index = 2;
            }
            return &D_8013FEB0[index];
        case 0x25:
            if (index >= 3) {
                index = 2;
            }
            return &D_8013FEC8[index];
    }
    return &D_8013F3D0[kind];
}
