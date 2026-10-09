#include "common.h"

typedef struct VTable VTable;

typedef struct {
    char reserved_00[8];
    short adjustment_08;
    short reserved_0A;
    void (*method_0C)(void *, s32);
} Methods;
typedef struct {
    char reserved_00[8];
    Methods *methods_08;
} Child;
typedef struct {
    char reserved_00[0x24];
    VTable *field_24;
    char reserved_28[0x78];
    Child *field_A0;
} State;
extern VTable D_8015A700;
extern void func_800EFD28(State *, s32);
extern void func_800A3918(State *);

void func_800FCEC4(State *state, s32 flags) {
    Child *child = state->field_A0;
    state->field_24 = &D_8015A700;
    if (child != 0) {
        Methods *methods = child->methods_08;
        methods->method_0C((char *)child + methods->adjustment_08, 3);
    }
    func_800EFD28(state, 0);
    if (flags & 1) {
        func_800A3918(state);
    }
}
