#include "func_801E981C_13B73C/types.h"

extern "C" void func_801E981C_13B73C(void) {
    s32 i;
    for (i = 0; i < 17; i++) {
        s32 value = 0;
        s32 slot;
        s32 state;
        Game800C9E00 *game = func_800C9E00();
        slot = D_80154718[i];
        state = game->slots[slot];
        /* The word-sized local starts as the slot byte and becomes occupancy.
         * The setter's u8 value parameter accounts for the call-site masks. */
        if (state == -1) {
            state = 0;
        } else {
            switch (state) {
            case 4:
                value = 1;
                break;
            case 5:
                /* ODD_C: restates the initial 0. The ROM keeps a distinct case-5 target
                 * (jump table 0x801F5DC4 -> 0x801E9898 `move s1,zero`); dropping this case
                 * sends it to the default target and measures 79 differing words. */
                value = 0;
                break;
            case 9:
            case 10:
            case 11:
                value = state - 9;
                break;
            case 12:
            case 13:
                value = state - 12;
                break;
            case 14:
            case 15:
                value = state - 14;
                break;
            }
            state = 1;
        }
        switch (slot) {
        case 0:
            func_801E9F90_13BEB0(0xA1, state);
            break;
        case 1:
            func_801E9F90_13BEB0(0xA2, state);
            break;
        case 2:
            func_801E9F90_13BEB0(0xA5, state);
            break;
        case 3:
            func_801E9F90_13BEB0(0xA6, state);
            break;
        case 4:
            func_801E9F90_13BEB0(0xA7, state);
            func_801E9F90_13BEB0(0xC6, value);
            break;
        case 5:
            func_801E9F90_13BEB0(0xA8, state);
            break;
        case 6:
            func_801E9F90_13BEB0(0xA9, state);
            break;
        case 7:
            func_801E9F90_13BEB0(0xAA, state);
            break;
        case 8:
            func_801E9F90_13BEB0(0xAB, state);
            func_801E9F90_13BEB0(0xC3, value % 2);
            func_801E9F90_13BEB0(0xC4, value / 2);
            break;
        case 9:
            func_801E9F90_13BEB0(0xAC, state);
            func_801E9F90_13BEB0(0xC5, value);
            break;
        case 10:
            func_801E9F90_13BEB0(0xAD, state);
            func_801E9F90_13BEB0(0xC7, 1 - value);
            break;
        case 11:
            func_801E9F90_13BEB0(0xA4, state);
            break;
        }
    }
}
