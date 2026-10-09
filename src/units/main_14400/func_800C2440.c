#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef struct Obj Obj;
typedef struct {
    u8 pad_00[0x18];
    short adjust_18;
    short reserved_1A;
    void (*initialize_1C)(void *);
} VTable;
struct Obj { u8 field_00; u8 pad_01[7]; VTable *vtable_08; };
/* Both direct callers install D_8014A7A8; slot 1C is func_800435D0,
 * whose only live input is self. The wrapper's public ABI has four arguments. */
void func_800C2440(Obj *obj, u8 value, s8 x, s8 y) {
    VTable *vtable = obj->vtable_08;
    obj->field_00 = value;
    vtable->initialize_1C((u8 *)obj + vtable->adjust_18);
}
