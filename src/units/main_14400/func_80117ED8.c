#include "common.h"

/* Original base vtable D_80153AA0 (rodata: adjustment words and function pointers),
 * modeled as one opaque whole object; only its address is stored here. */
typedef struct VTable VTable;

typedef struct {
    char reserved_00[8];
    const VTable *field_08;
} State;
extern const VTable D_80153AA0;
extern void func_800AC68C(State *);

void func_80117ED8(State *state, s32 flags) {
    state->field_08 = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(state);
    }
}
