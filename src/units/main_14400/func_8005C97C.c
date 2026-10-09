#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

/* Screen fade state (see func_8005C890/func_8005C910). */
typedef struct {
    s16 total;
    s16 remaining;
    u8 color;
    u8 mode;
} Fade;

extern Fade D_80165952;

void func_8005C814(u32 value0, u32 value1, u32 value2, u32 value3);
void func_8005C870(u8 v);

s32 func_8005C97C(void) {
    u8 color = D_80165952.color;
    s32 target = D_80165952.total;

    if (target == 0) {
        return 0;
    }
    if (D_80165952.mode != 0) {
        D_80165952.remaining++;
    } else {
        target = 0;
        D_80165952.remaining--;
    }
    if (target == D_80165952.remaining) {
        D_80165952.total = 0;
        func_8005C814(color, color, color, D_80165952.mode * 255);
        if (D_80165952.mode == 0) {
            func_8005C870(0);
        }
        return 0;
    }
    func_8005C814(color, color, color, (u8)((D_80165952.remaining * 255) / D_80165952.total));
    return 1;
}
