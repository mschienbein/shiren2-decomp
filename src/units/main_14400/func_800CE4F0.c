#include "common.h"
/* ItemSet-family slot 0x60: CE8F4/CEF3C/CFFC8/D04F4 return s32;
 * CFFC8 and D04F4 consume both the element pointer and signed index. */
typedef struct {
    unsigned char pad_00[0x60];
    short adjustment_60;
    short reserved_62;
    s32 (*method_64)(void *self, void *element, s32 index);
} VTable800CE4F0;
typedef struct { void *pool_00; VTable800CE4F0 *vtable_04; } Object800CE4F0;
s32 func_800CE4F0(Object800CE4F0 *object, void *element, s32 index)
{
    return object->vtable_04->method_64((unsigned char *)object + object->vtable_04->adjustment_60, element, index);
}
