#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef struct {
    short delta;
    short index;
    s32 (*call)(void *obj, s32 mode, s32 a, u8 b, s32 c);
} Method;
typedef struct {
    u8 pad_00[0x90];
    Method field_90;
} VTable;
typedef struct Obj {
    u8 pad_00[0x24];
    const VTable *field_24;
} Obj;
extern s32 func_800E1CC4(Obj *obj, s32 kind);

static inline void apply_amount(Obj *obj, s8 amount) {
    const Method *method = &obj->field_24->field_90;
    method->call((char *)obj + method->delta, 0, amount > 0 ? 0x12 : 0x11, 0xFE, amount);
}

/* Item vtable slot +0x44 (void result): `unused` is the adjusted receiver the dispatcher supplies. */
void func_80118E10(void *unused, Obj *obj) {
    apply_amount(obj, func_800E1CC4(obj, 3) ? 2 : 1);
}
