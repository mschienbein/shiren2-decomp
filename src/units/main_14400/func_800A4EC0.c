#include "common.h"

extern void func_800A665C(void *, void *);
extern void func_800A4F58(void *, void *);

void func_800A4EC0(void *state, void *value) {
    func_800A665C(state, value);
    func_800A4F58(state, value);
}
