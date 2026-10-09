#include "common.h"
/* Writable view of the whole 0x40-byte RNG object at D_80147620 that func_800C5D70 constructs
 * (header, three state words at +0x10, three saved 12-byte states at +0x1C). */
typedef struct {
    void *state_00;
    void *saved_04;
    s32 depth_08;
    const void *vtable_0C;
    u32 state_10[3];
    u32 saved_1C[9];
} RngObject;
extern void *func_800C5D70(void *object);
extern RngObject D_80147620;
/* The constructor's returned receiver is intentionally discarded. */
void func_800C5F04(void) { func_800C5D70(&D_80147620); }
