#include "common.h"

typedef struct VTable8004645C VTable8004645C;

typedef struct {
    char pad0[0x18];
    VTable8004645C *vtbl;
} Obj8004645C;

struct VTable8004645C {
    char pad0[0x28];
    short adjust28;
    void (*func2C)(char *self, s32 arg1, signed char *out);
};

void func_8004645C(s32 *result, Obj8004645C *obj) {
    signed char flag;
    VTable8004645C *vtbl = obj->vtbl;

    vtbl->func2C((char *)obj + vtbl->adjust28, 1, &flag);
    *result = flag != 0;
}
