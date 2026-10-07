#include "common.h"

typedef struct {
    unsigned char pad00[0xA];
    unsigned short field0A;
} Record_80082874;

extern unsigned short D_801A9F58;
extern unsigned short D_801A9F5A;
extern Record_80082874 *D_8013E81C;

void func_80082998(void);

void func_80082874(s32 dx, s32 dy)
{
    D_801A9F58 += dx;
    if ((short)D_801A9F58 >= D_8013E81C->field0A * 8) {
        func_80082998();
    } else if ((short)D_801A9F58 < 0) {
        D_801A9F58 = 0;
    }
    D_801A9F5A += dy;
}
