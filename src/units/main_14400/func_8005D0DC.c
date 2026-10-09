#include "common.h"
typedef struct { short field0, field2, field4, field6; unsigned char pad8[6]; unsigned char fieldE, fieldF, field10, field11; unsigned char pad12[0xE]; } View;
typedef struct { float left, top, right, bottom; } Bounds;
extern View D_80165960;
void func_8005D0DC(Bounds *out) {
    float scaled = D_80165960.field0;
    float height;
    scaled = (double)scaled * 0.03125;
    if (D_80165960.fieldF) scaled = scaled * D_80165960.fieldE / D_80165960.fieldF;
    scaled = (double)scaled * 32.0;
    out->left = (float)D_80165960.field4 - scaled;
    scaled = D_80165960.field0;
    if (D_80165960.fieldF) scaled = scaled * D_80165960.fieldE / D_80165960.fieldF;
    out->right = (float)D_80165960.field4 + scaled;
    height = D_80165960.field2;
    if (D_80165960.field11) height = height * D_80165960.field10 / D_80165960.field11;
    out->top = (float)D_80165960.field6 - height;
    out->bottom = (float)D_80165960.field6 + height;
}
