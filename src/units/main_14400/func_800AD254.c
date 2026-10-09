#include "common.h"

typedef struct {
    unsigned char field_00;
    unsigned char field_01;
    unsigned char field_02;
} State;
extern s32 D_80147620[];
extern void func_800AD188(unsigned char, unsigned char *, unsigned char *);
extern s32 func_800C587C(void *, unsigned char);
extern s32 func_800AD37C(unsigned char);
extern void func_800AD3F4(unsigned char);
extern void func_800AD42C(unsigned char);
extern void func_800AD308(unsigned char);
extern void func_800ACF34(State *);

static inline s32 is_available(unsigned char kind) {
    return func_800AD37C(kind) ^ 1;
}

void func_800AD254(State *state) {
    unsigned char first;
    unsigned char second;
    s32 selected;
    func_800AD188(state->field_00, &first, &second);
    selected = func_800C587C(D_80147620, first);
    if (is_available(state->field_01)) {
        if (selected != 0) {
            func_800AD3F4(state->field_01);
        } else {
            func_800AD42C(state->field_01);
        }
        func_800AD308(state->field_01);
    }
    if (second != 0) {
        func_800ACF34(state);
    } else {
        state->field_02 &= 0xFD;
    }
}
