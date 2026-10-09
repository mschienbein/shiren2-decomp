#include "common.h"

typedef struct {
    unsigned char pad0[0x54];
    s32 field54;
    /* func_8009F680 initializes the 0xA2-entry fixed remap table. */
    unsigned char field58[0xA2];
} Object;

s32 func_8009F8F8(Object *self, s32 index)
{
    if (self->field54 != 0) {
        return index;
    } else {
        return self->field58[index];
    }
}
