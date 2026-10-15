#include "func_801EB348_13D268/types.h"
extern s32 func_801ECF5C_13EE7C(const u8 *text, s32 index);
extern void func_801EE8CC_1407EC(s32 final);

void func_801EB348_13D268(const u8 *text)
{
    s32 index = -1;
    while (func_801ECF5C_13EE7C(text, ++index)) {
    }
    func_801EE8CC_1407EC(1);
}
