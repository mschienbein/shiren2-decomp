#include "common.h"
typedef unsigned char u8;
typedef struct { float values[8]; } Parameters;
typedef struct { short field0, field2, field4; } State;
extern Parameters D_80165364;
extern s32 D_80165384, D_8016539C;
extern void func_800598B4(u8 a, u8 b, u8 c, u8 d);
void func_8005C59C(void *a, void *b, void *c, void *d) {
    Parameters *parameters = a;
    u8 *enabled = b, *color = d;
    State *state = c;
    if (*enabled) {
        D_80165364.values[0] = parameters->values[0];
        D_80165364.values[1] = parameters->values[1];
        D_80165364.values[2] = parameters->values[2];
        D_80165364.values[3] = parameters->values[3];
        D_80165364.values[4] = parameters->values[4];
        D_80165364.values[5] = parameters->values[5];
        D_80165364.values[6] = parameters->values[6];
        D_80165364.values[7] = parameters->values[7];
    }
    D_80165384 = *enabled;
    D_8016539C = state->field4;
    func_800598B4(color[0], color[1], color[2], color[3]);
}
