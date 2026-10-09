#include "common.h"

typedef struct {
    unsigned char pad00[0x48];
    short adjust48;
    short pad4A;
    s32 (*value4C)(void *);
} VTable;
typedef struct { s32 field00; void *field04; s32 active08; VTable *vtable0C; s32 field10; void *field14; } Obj;
extern void *func_800C9E00(void);
extern s32 func_800D1488(void *self, unsigned char arg);

s32 func_80091C10(Obj *obj)
{
    void *self = func_800C9E00();
    if (obj->field14 != 0) {
        s32 value = obj->vtable0C->value4C((char *)obj + obj->vtable0C->adjust48);
        if (func_800D1488(self, value) >= 0) {
            obj->active08 = 1;
            return obj->vtable0C->value4C((char *)obj + obj->vtable0C->adjust48);
        }
    }
    return 0;
}
