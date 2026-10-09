#include "common.h"

typedef struct Obj {
    void *pool_00;
    const void *vtable_04;
} Obj;
extern const unsigned char D_801544C0[132];
Obj *func_800CFEEC(Obj *self) {
    self->vtable_04 = D_801544C0;
    return self;
}
