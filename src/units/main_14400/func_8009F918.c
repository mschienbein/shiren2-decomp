#include "common.h"

typedef struct {
    unsigned char pad0[0x54];
    s32 field_54;
} Menu;

void func_8009F950(Menu *menu, s32 input);
void func_8009D9E0(void *p, s32 mode);

void func_8009F918(Menu *menu, s32 input)
{
    if (menu->field_54 != 0) {
        func_8009F950(menu, input);
    } else {
        func_8009D9E0(menu, input);
    }
}
