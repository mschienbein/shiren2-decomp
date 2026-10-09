#include "common.h"
/* D_80158C98+0x94 names func_800E115C: signed mode/key, byte value,
 * and signed final argument; preserve the result of that virtual method. */
typedef struct {
    unsigned char pad_00[0x90];
    short adjustment_90;
    short reserved_92;
    s32 (*method_94)(void *self, s32 mode, s32 key, unsigned char value, s32 extra);
} VTable800E3228;
typedef struct { unsigned char pad_00[0x24]; VTable800E3228 *vtable_24; } Object800E3228;
s32 func_800E3228(Object800E3228 *object)
{
    return object->vtable_24->method_94((unsigned char *)object + object->vtable_24->adjustment_90, 1, 0, 0, 0);
}
