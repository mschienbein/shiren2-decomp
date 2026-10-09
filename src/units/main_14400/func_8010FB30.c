#include "common.h"

typedef struct { s32 field_00, field_04; } Pair;
typedef struct { unsigned char value; } Dir;
typedef struct { unsigned char field_00, field_01; } State;
typedef struct { void *field_00; Pair field_04; unsigned char field_0C; unsigned char fields_0D[0x16]; unsigned char field_23; } Context;
extern s32 func_80049CB4(s32, ...);
extern void *func_800A2594(Pair *, void *, Dir);
extern void func_800A58FC(void *, Pair *);
extern void func_800A59A4(void *);

void func_8010FB30(State *state, Context *context)
{
    Pair position;
    if (context->field_23) {
        Dir opposite;
        Pair *where;
        Pair *point;
        opposite.value = (context->field_0C + 4) & 7;
        func_800A2594(&position, &context->field_04, opposite);
        where = &position;
        /* ODD_C: the destination pointer is copied before the first branch, keeping the
           announcement pointer and the final move target in separate saved registers. */
        point = where;
        func_80049CB4(0x1048, context->field_00, where);
        if (state->field_01 == 0x44) {
            func_80049CB4(6);
            func_80049CB4(0x49, context->field_00, where);
            func_80049CB4(7);
        }
        func_80049CB4(6);
        func_80049CB4(0x12D);
        if (state->field_01 != 0x44) func_80049CB4(0x49, context->field_00, where);
        func_800A59A4(context->field_00);
        func_800A58FC(context->field_00, point);
        func_80049CB4(7);
    }
}
