#include "common.h"

typedef struct {
    s32 field_00;
    void *field_04;
} Object;

extern void func_800CA088(void *);
extern void func_800CA2C0(void *);
extern void func_800CB408(Object *);

void func_800CB3C8(Object *object) {
    func_800CA088(object->field_04);
    func_800CA2C0(object->field_04);
    object->field_00 = 1;
    func_800CB408(object);
}
