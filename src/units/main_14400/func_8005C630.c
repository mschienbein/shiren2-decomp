#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u16 fields[3]; } Parameters;
extern float D_80165364[8];
extern s32 D_80165384;
extern s32 D_8016539C;
extern void func_80059954(u8 *a, u8 *b, u8 *c, u8 *d);

void func_8005C630(float *values, u8 *active, Parameters *params, u8 *rect)
{
    if (D_80165384 != 0) {
        values[0] = D_80165364[0];
        values[1] = D_80165364[1];
        values[2] = D_80165364[2];
        values[3] = D_80165364[3];
        values[4] = D_80165364[4];
        values[5] = D_80165364[5];
        values[6] = D_80165364[6];
        values[7] = D_80165364[7];
    }
    *active = D_80165384;
    params->fields[2] = D_8016539C;
    func_80059954(rect, rect + 1, rect + 2, rect + 3);
}
