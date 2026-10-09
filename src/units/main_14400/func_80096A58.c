#include "common.h"

/* The three 0x5C-byte menus (zero-initialized .data defined with func_800968BC);
 * their vtable pointer lives at +0x4C. */
typedef struct { unsigned char pad_00[0x4C]; const void *vtable_4C; unsigned char pad_50[0xC]; } Menu80096A58;

extern Menu80096A58 D_80140278, D_80140304, D_80140390;
/* Base menu vtable. */
extern const unsigned char D_80151E38[144];

/* Restores the base vtable of each menu. */
void func_80096A58(void) {
    s32 local[4];
    D_80140390.vtable_4C = D_80151E38;
    D_80140304.vtable_4C = D_80151E38;
    D_80140278.vtable_4C = D_80151E38;
}
