#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad_00[0x5C]; void *field_5C; } Object;
/* The method forwards its member even though the callee currently ignores it. */
extern void func_80048614(void *object);

void func_80096B68(Object *object) {
    func_80048614(object->field_5C);
}
