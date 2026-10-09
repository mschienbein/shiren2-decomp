#include "common.h"
typedef struct { unsigned char fields00[0x28]; short field28; void (*field2C)(void *, s32, unsigned char *); } Methods;
typedef struct { unsigned char fields00[0x18]; Methods *field18; } Context;
typedef struct { unsigned char fields00[0x10]; unsigned char field10; unsigned char field11; } Object;
extern const char D_8015ED7C[16];
extern void func_800CA4E8(Context *context, void *type);
extern void func_80111810(Object *object, Context *context);
void func_8011DAE0(Object *object, Context *context) {
    unsigned char value;
    unsigned char low;
    Methods *methods;
    func_800CA4E8(context, (void *)D_8015ED7C);
    func_80111810(object, context);
    methods = context->field18;
    methods->field2C((unsigned char *)context + methods->field28, 1, &value);
    low = value & 0x7F;
    object->field10 = low;
    if (low == 0) {
        object->field10 = 0;
    }
    object->field11 = value >> 7;
}
