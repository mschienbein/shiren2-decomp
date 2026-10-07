#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 field_00[0x12];
    u8 field_12;
    u8 field_13;
    u8 field_14;
} State;
typedef struct { float x0; float y0; float x1; float y1; } Rect;
extern State D_80165960[];
extern void func_8005D0DC(Rect *);

void func_8005D79C(void) {
    Rect rectangle;
    Rect *rect = &rectangle;
    State *state = D_80165960;
    if (state->field_14 && state->field_12 == state->field_13 && !(state->field_14 & 0x20)) {
        func_8005D0DC(rect);
        if (rect->x0 >= rect->x1 || rect->y0 >= rect->y1) {
            return;
        }
        if (rect->x0 < 12.0f) rect->x0 = 12.0f;
        if (rect->y0 < 10.0f) rect->y0 = 10.0f;
        if (rect->x1 > 308.0f) rect->x1 = 308.0f;
        if (rect->y1 > 230.0f) rect->y1 = 230.0f;
    }
}
