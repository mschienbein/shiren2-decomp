#include "common.h"
typedef struct { short field_00, field_02; unsigned char field_04, field_05; } State;
extern State D_80165952;
extern void func_8005C814(u32 value0, u32 value1, u32 value2, u32 value3);
extern void func_8005C870(unsigned char value);
void func_8005C890(s32 enabled, s32 value, s32 timer)
{
    D_80165952.field_00 = timer;
    D_80165952.field_05 = enabled;
    if (enabled) {
        D_80165952.field_04 = value;
        D_80165952.field_02 = 0;
    } else {
        D_80165952.field_02 = timer;
    }
    if (D_80165952.field_00 == 0) {
        func_8005C814(value, value, value, enabled * 255);
        if (!D_80165952.field_05) return;
    }
    func_8005C870(2);
}
