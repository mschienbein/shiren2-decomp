#include "common.h"

typedef unsigned char u8;

/* 0xB0-byte unit record shared by the five unit tables. */
typedef struct Unit {
    u8 data[0xB0];
} Unit;

extern Unit D_801DEAB4[30];
extern Unit D_801D8FFC[4];
extern Unit D_801D2C2C[30];
extern Unit D_801E02A8[110];
extern Unit D_801DD378[32];

Unit *func_8007946C(s32 side, s32 slot) {
    Unit *unit = 0;
    switch (side) {
    case 0:
        if ((u32)slot < 30) unit = &D_801DEAB4[slot];
        break;
    case 1:
        if ((u32)slot < 4) unit = &D_801D8FFC[slot];
        break;
    case 2:
        if ((u32)slot < 30) unit = &D_801D2C2C[slot];
        break;
    case 3:
        if ((u32)slot < 110) unit = &D_801E02A8[slot];
        break;
    case 4:
        if ((u32)slot < 32) unit = &D_801DD378[slot];
        break;
    }
    return unit;
}
