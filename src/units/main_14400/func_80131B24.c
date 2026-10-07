#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/*
 * The four 10-byte channel slots at D_801D4CDC (same object and stride as the
 * views in func_80131820 and func_80131A70): +4 timer, +6 mode, +7 state.
 */
typedef struct {
    u16 x;
    u16 y;
    u16 timer;
    u8 mode;
    u8 state;
    u8 field_8;
    u8 field_9;
} Slot80131B24;

extern Slot80131B24 D_801D4CDC[4];

/* The D_80148E44 request-handler contract passes an unused message pointer. */
s32 func_80131B24(void *message) {
    u32 i;

    for (i = 0; i < 4; i++) {
        D_801D4CDC[i].mode = 4;
        D_801D4CDC[i].state |= 0x80;
    }
    return 0;
}
