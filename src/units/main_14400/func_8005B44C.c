#include "common.h"

typedef struct { float x, y, z; } Vector;
typedef struct {
    Vector field0;
    Vector field0C;
    Vector field18;
    float field24;
    float field28;
} State;
extern Vector D_80165300;
extern Vector D_80165324;
extern float D_8016533C;
extern float D_80165340;

void func_8005B44C(State *state)
{
    D_80165300 = state->field0;
    D_80165324 = state->field18;
    D_8016533C = state->field24;
    D_80165340 = state->field28;
}
