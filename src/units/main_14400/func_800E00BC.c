#include "common.h"
typedef struct VTable VTable;
typedef struct Obj {
    unsigned short kind_00;
    unsigned short pad_02;
    const VTable *vtable_04;
    unsigned char field_08;
} Obj;
extern const VTable D_80157FA8, D_80158C40;
Obj *func_800E00BC(Obj *self, unsigned char *value) {
    self->vtable_04 = &D_80157FA8;
    self->kind_00 = 62;
    self->vtable_04 = &D_80158C40;
    self->field_08 = *value;
    return self;
}
