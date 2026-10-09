#include "common.h"

typedef struct {
    void *field_00;
    void *field_04;
} State;
extern void *func_800E215C(void *);
extern void *func_800EBA54(void *);

void func_800D0204(State *state, void *source) {
    state->field_04 = func_800E215C(source);
    state->field_00 = func_800EBA54(source);
}
