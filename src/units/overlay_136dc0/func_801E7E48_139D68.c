#include "func_801E7E48_139D68/types.h"

extern u8 *func_8005DB10(void);
extern void func_8005DB20(void);
extern void func_800265E0(void *dst, s32 length);
extern void func_8005D974(s32 x, s32 y, s32 x0, s32 y0, s32 first, s32 second);
extern void func_8005D90C(unsigned char value, unsigned char flag);
extern void func_8005D8F8(s32 value);

void func_801E7E48_139D68(s32 cmd, s32 a, s32 b, s32 c, s32 d) {
    Obj80165960 *p = (Obj80165960 *)func_8005DB10();

    switch (cmd) {
    case 0:
        p->x14 |= 1;
        break;
    case 1:
        p->x14 ^= 0x10;
        break;
    case 2:
        func_8005DB20();
        break;
    case 3:
        func_800265E0(p, sizeof(Obj80165960));
        p->x4 = a;
        p->x6 = b;
        p->x1A = c;
        break;
    case 4:
        p->x0 = a;
        p->x2 = b;
        p->x18 = c;
        p->x19 = d;
        func_8005D974(p->x4, p->x6, p->x0, p->x2, p->x18, p->x19);
        break;
    case 5:
        func_8005D90C(a, b);
        break;
    case 6:
        func_8005D8F8((u8)a);
        break;
    case 7:
        p->xF = a;
        if (b != 0) {
            p->xE = a;
            p->x14 |= 4;
        } else {
            p->xE = 0;
            p->x14 &= ~4;
        }
        break;
    case 8:
        p->x11 = a;
        if (b != 0) {
            p->x10 = a;
            p->x14 |= 8;
        } else {
            p->x10 = 0;
            p->x14 &= ~8;
        }
        break;
    }
}
