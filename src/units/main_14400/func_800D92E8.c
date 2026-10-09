#include "common.h"

typedef short s16;

typedef struct {
    s16 field_0;
    void *vtable;
} Obj;

extern u32 D_80157FA8[];
extern u32 D_80158008[];

Obj *func_800D92E8(Obj *self, unsigned char *unused_payload) {
    self->vtable = D_80157FA8;
    self->field_0 = 6;
    self->vtable = D_80158008;
    return self;
}
