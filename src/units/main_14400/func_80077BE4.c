#include "common.h"
typedef struct { short field_00, field_02, field_04, field_06, field_08, field_0A; } Entry;
Entry D_8013D904[2] = { {-1, -1, 0, 0, 0, 0}, {-1, -1, 0, 0, 0, 0} };
void func_80077BE4(s32 kind, s32 slot, s32 value) {
    s32 index;
    switch (kind) {
    case 0x17: index = 0; break;
    case 0x1A: index = 1; break;
    default: return;
    }
    if (slot == 3) D_8013D904[index].field_08 = value;
    else if (slot == 4) D_8013D904[index].field_0A = value;
}
