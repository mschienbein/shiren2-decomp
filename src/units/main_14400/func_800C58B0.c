#include "common.h"
typedef struct { unsigned char pad_00[8]; short adjust_08; short pad_0A; s32 (*method_0C)(void *); } VTable;
typedef struct { unsigned char pad_00[12]; const VTable *field_0C; } Object;
/* Slot 0x0C: RNG implementations func_800C5CA4/func_800C5E10 return a signed word; this
 * wrapper narrows it to an unsigned halfword (0x800C58D0). */
unsigned short func_800C58B0(Object *object) {
    return object->field_0C->method_0C((char *)object + object->field_0C->adjust_08) & 0xFFFF;
}
