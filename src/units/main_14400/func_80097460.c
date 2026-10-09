#include "common.h"

typedef struct Menu { unsigned char pad_00[0x58]; void *field_58; } Menu;
typedef Menu Obj;
extern void func_8009567C(Menu *menu, s32 dir);
extern void func_80097498(Menu *menu, s32 dir);
void func_80097460(Obj *obj, s32 direction)
{
    if (obj->field_58 != 0) {
        func_8009567C(obj, direction);
    } else {
        func_80097498(obj, direction);
    }
}
