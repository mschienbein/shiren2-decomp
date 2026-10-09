#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x2C];
    s32 mode2C;
} Obj80045250;

/* Last sampled tick counter and the carried remainder of the tick-to-unit division. */
extern u32 D_80138BC0;
extern u32 D_80138BC4;

u32 func_8006C550(void);
u8 func_8006C560(void);
void func_800CB000(Obj80045250 *obj, u32 amount);
void func_800CB050(Obj80045250 *obj);

void func_800452C0(Obj80045250 *obj)
{
    if (obj->mode2C == 1) {
        u32 now = func_8006C550();
        u32 elapsed = now - D_80138BC0;
        u8 rate;
        u32 carry;

        D_80138BC0 = now;
        rate = func_8006C560();
        carry = D_80138BC4 + elapsed % rate;
        D_80138BC4 = carry % rate;
        func_800CB000(obj, elapsed / rate + carry / rate);
        func_800CB050(obj);
    }
}
