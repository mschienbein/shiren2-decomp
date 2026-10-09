#include "common.h"

typedef struct {
    char pad0[0x5C];
    /* func_80048728 reads/writes the handle at subobject +0xC. */
    char field_5C[0x10];
} Obj_8009D5C8;

extern void func_80048728(void *);

void func_8009D5C8(Obj_8009D5C8 *obj) {
    func_80048728(obj->field_5C);
}
