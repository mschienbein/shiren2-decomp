#include "common.h"
typedef struct { unsigned char field_0[12]; float field_C, field_10, field_14; } Object;
extern Object *D_8013B984;
void func_80067578(float *x, float *y, float *z) { if (D_8013B984) { if (x) *x = D_8013B984->field_C; if (y) *y = D_8013B984->field_10; if (z) *z = D_8013B984->field_14; } else { if (x) *x = 0; if (y) *y = 0; if (z) *z = 0; } }
