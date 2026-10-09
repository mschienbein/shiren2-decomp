#include "common.h"
typedef struct {
    unsigned char pad_00[8]; const void *vtable_08;
    unsigned char flags_0C; unsigned char pad_0D[2]; unsigned char value_0F;
} Obj801282D0;
extern const unsigned char D_80160720[];
extern const unsigned char D_80153AA0[];
extern void func_800D788C(unsigned char *self);
extern void func_800AC68C(void *self);
static __inline__ s32 second_flag(u32 state) {
    return state & 2;
}
static __inline__ s32 fourth_flag(u32 state) {
    return state & 4;
}
static __inline__ s32 flag_set(u32 flag) {
    return flag != 0;
}
static __inline__ s32 active_flags(u32 state) {
    return second_flag(state) && flag_set(fourth_flag(state));
}
void func_801282D0(Obj801282D0 *self, s32 flags) {
    u32 state = self->flags_0C;
    self->vtable_08 = D_80160720;
    if (!(state & 8)) {
        s32 active = active_flags(state);
        if (active) {
            self->value_0F >>= 1;
            func_800D788C((unsigned char *)self);
        }
    }
    self->vtable_08 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(self);
    }
}
