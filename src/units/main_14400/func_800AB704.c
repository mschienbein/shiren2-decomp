#include "common.h"

typedef unsigned short u16;

/* Whole 0x40-byte RNG: three state words and three saved 12-byte states. */
typedef struct {
    u32 *state_begin;
    u32 *saved_states;
    s32 depth_8;
    void *vtable_C;
    u32 state_10[3];
    u32 saved_1C[9];
} Rng800AB704;
extern Rng800AB704 D_80147620;
extern u16 D_801569BE;
extern u16 D_801569C0;
s32 func_800C5954(Rng800AB704 *arg0, u16 arg1, u16 arg2);
void func_800AE6C4(void *arg0, u16 arg1);
void func_800AB704(void *arg0) {
    func_800AE6C4(arg0, func_800C5954(&D_80147620, D_801569BE, D_801569C0));
}
