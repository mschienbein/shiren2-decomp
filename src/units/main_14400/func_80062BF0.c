#include "common.h"

extern s32 D_8016DB18; /* map mode */
void func_80066950(s32 *a, s32 *b, s32 *c, s32 *d);

void func_80062BF0(s32 *a, s32 *b, s32 *c, s32 *d)
{
    switch (D_8016DB18) {
    case 1:
    case 2:
        func_80066950(a, b, c, d);
        break;
    case 0:
    case 3:
    case 4:
        if (a != 0) {
            *a = 0;
        }
        if (b != 0) {
            *b = 0;
        }
        if (c != 0) {
            *c = 0x37;
        }
        if (d != 0) {
            *d = 0x21;
        }
        break;
    }
}
