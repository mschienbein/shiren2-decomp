#include "common.h"
typedef struct VTable VTable;
typedef struct { unsigned char pad0[8]; const VTable *vtable8; } S;
extern void *func_800AC0C0(S *self, s32 a, s32 b);
extern const VTable D_8015D300;
S *func_8010E020(S *object, s32 value) {
    func_800AC0C0(object, 0xE, value);
    object->vtable8 = &D_8015D300;
    return object;
}
