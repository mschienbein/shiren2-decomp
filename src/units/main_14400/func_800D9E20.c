#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad_00[0x98]; short delta_98; short index_9A; u8 *(*method_9C)(u8 *); } Methods;
typedef struct { u8 pad_00[0x24]; Methods *field_24; } Obj;
extern Obj *D_801476B8;
extern void func_800CDE78(void *container);
/* The action slot supplies self, but this action operates only on the global actor. */
s32 func_800D9E20(void *self)
{
    Obj *obj = D_801476B8;
    func_800CDE78(obj->field_24->method_9C((u8 *)obj + obj->field_24->delta_98));
    return 1;
}
