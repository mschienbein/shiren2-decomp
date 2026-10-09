#include "common.h"
typedef struct { unsigned char field_00[10]; unsigned char field_0A; } Object;
extern void func_800A7B18(void *target, void *source, s32 amount, s32 kind);
extern void func_80117000(void *object, void *source, void *target);
void func_8011885C(void *object, void *source, Object *target) {
    if (target->field_0A == 0x28) {
        func_800A7B18(target, source, 0x32, 6);
    } else {
        func_80117000(object, source, target);
    }
}
