#include "common.h"

void *func_80062CCC(void *value, void *base, u32 tag)
{
    if ((((s32)value >> 24) & 0xF) != tag) {
        return value;
    }
    return (void *)(((u32)value & 0xFFFFFF) + (u32)base); /* local-arithmetic-qualification: pointer form emits addu v0,a1,v0 */
}
