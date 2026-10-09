#include "common.h"

typedef struct { unsigned char unknown00[0x54]; unsigned char field54; } Object;
extern unsigned short D_8014767C;
extern Object *D_801476B8;
extern s32 func_80046240(void);
extern s32 func_800C93CC(void);
extern void func_800C8D78(Object *, short);
static __inline__ s32 is_blocked(void) {
    s32 blocked = 0;
    if (func_80046240() || ((D_8014767C >> 6) & 1) || func_800C93CC()) blocked = 1;
    return blocked;
}
void func_800C86C8(Object *object, s32 mode, short value) {
    s32 blocked = is_blocked();
    if (!blocked) {
        s32 amount;
        if (mode == 2) object->field54 |= 2;
        amount = (short)value;
        func_800C8D78(D_801476B8, amount);
        if (object != D_801476B8) func_800C8D78(object, amount);
    }
}
