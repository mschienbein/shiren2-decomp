#include "common.h"
extern unsigned char D_80169AFC[];
extern signed char D_8016AB04[];
extern unsigned char func_800416F8(s32, s32), func_80041DDC(s32, s32);
extern s32 func_80041DBC(unsigned char);
extern signed char func_800421D4(s32, s32), func_800423D0(s32, s32);
extern s32 func_80042260(s32 x, s32 y);
extern s32 func_800422B0(s32, s32);
extern unsigned char func_80042444(s32 x, s32 y);
void func_80061FB8(s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 x = x0, y, rowState, rowTile;
    if (x <= x1) {
        rowState = x * 108;
        rowTile = x * 54;
        do {
        unsigned char *tile;
        signed char *state, *extra;
        y = y0;
        { unsigned char *tileBase = y + D_80169AFC; signed char *stateBase = y * 2 + D_8016AB04;
          tile = (unsigned char *)(rowTile + (s32)tileBase);
          state = (signed char *)(rowState + (s32)stateBase);
        }
        if (y <= y1) {
        extra = state + 1;
        do {
            *tile = func_800416F8(x, y);
            *extra = 0;
            if (*tile) {
                switch (func_80041DBC(*tile)) {
                case 2: *state = func_800421D4(x, y); break;
                case 20:
                    switch (*tile) {
                    case 0xF5: *state = func_800423D0(x, y); break;
                    case 0xF6: *state = (signed char)func_80042444(x, y); break;
                    default: *state = 0; break;
                    }
                    break;
                case 16:
                    switch (*tile) {
                    case 0xD5: break;
                    case 0xE7: *state = (signed char)func_80042260(x, y); break;
                    default: *state = 0; break;
                    }
                    *extra = (signed char)func_800422B0(x, y); break;
                case 19:
                    if (*tile == 0xF2) { *tile = func_80041DDC(x, y); *state = 0; }
                    break;
                default: *state = 0; break;
                }
            }
            y++; tile++; extra += 2; state += 2;
        } while (y <= y1);
        }
        rowState += 108; x++; rowTile += 54;
        } while (x <= x1);
    }
}
