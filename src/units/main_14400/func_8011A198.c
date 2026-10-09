#include "common.h"

typedef struct VTable VTable;
typedef struct {
    unsigned char pad_00[8];
    const VTable *field_08;
} Obj;
extern const VTable D_80153AA0;
extern void func_800AC68C(void *a);

void func_8011A198(Obj *obj, s32 flags) {
    obj->field_08 = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
