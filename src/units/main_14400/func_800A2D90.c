#include "common.h"
typedef unsigned char u8;
s32 func_800A2D90(u8 kind, u8 value) {
    if (kind == 3) {
        switch (value) {
        case 9: case 10: case 0x48: case 0xf7: return 1;
        }
    } else if (kind == 4) {
        switch (value) {
        case 9: case 10: case 0x5d: case 0x5f: case 0x78:
        case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xf7: return 1;
        }
    }
    return 0;
}
