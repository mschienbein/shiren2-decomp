#include "common.h"

typedef unsigned short u16;

/* func_800C5D70 brackets this three-word state array with begin/end pointers. */
typedef struct {
    u32 *state_begin;
    u32 *state_end;
    u32 unknown_8;
    void *vtable_C;
    u32 state_10[3];
} Rng800AB704;
extern Rng800AB704 D_80147620;
extern u16 D_801569BE;
extern u16 D_801569C0;
s32 func_800C5954(Rng800AB704 *arg0, u16 arg1, u16 arg2);
void func_800AE6C4(void *arg0, u16 arg1);
void func_800AB704(void *arg0) {
    func_800AE6C4(arg0, func_800C5954(&D_80147620, D_801569BE, D_801569C0));
}
