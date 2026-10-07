#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/*
 * The four 10-byte channel slots occupy [0x801D4CDC, 0x801D4D04), as in
 * func_80131B24. The splat label D_801D4CE3 is slot 0's +7 state member,
 * not a second object; all four accesses stay within this containing array.
 */
typedef struct {
    u16 x;
    u16 y;
    u16 timer;
    u8 mode;
    u8 state;
    u8 field_8;
    u8 field_9;
} Slot80131B6C;

extern Slot80131B6C D_801D4CDC[4];

/* The D_80148E44 request-handler contract passes an unused message pointer. */
s32 func_80131B6C(void *message) {
    u32 i;

    for (i = 0; i < 4; i++) {
        D_801D4CDC[i].state &= 0x7F;
    }
    return 0;
}
