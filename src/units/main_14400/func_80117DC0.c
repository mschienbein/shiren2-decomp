#include "common.h"

typedef struct { unsigned char unknown00[0x78]; short adjust78; short unknown7a; void (*method7c)(void *, short); } VTable;
typedef struct { unsigned char unknown00[0x24]; VTable *field24; } Object;
extern unsigned short D_801569E4;
extern s32 func_800A99D0(void);
extern void func_800498E4(s32, ...);
extern s32 func_800E1CC4(Object *, s32);
/* Item vtable slot +0x44: `unused` is the adjusted receiver the dispatcher supplies; not used here. */
void func_80117DC0(void *unused, Object *object) {
    if (func_800A99D0()) {
        func_800498E4(0x222);
    } else {
        s32 multiplier;
        VTable *table;
        multiplier = func_800E1CC4(object, 3) ? 2 : 1;
        table = object->field24;
        table->method7c((char *)object + table->adjust78, (short)(D_801569E4 * multiplier));
    }
}
