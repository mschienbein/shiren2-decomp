#include "common.h"
typedef struct Target Target;
/* 0xE0-byte object (func_8010A07C allocation); the 0x18-byte collection at +0xC4 is built by func_800CEC90. */
typedef struct { unsigned char pad_00[0xC4]; unsigned char collection_C4[0x18]; } Object8010A1C4;
void func_800EE9C0(void *object, Target *target);
void func_800CE9C4(void *collection, Target *target);
void func_8010A1C4(Object8010A1C4 *object, Target *target)
{
    func_800EE9C0(object, target);
    func_800CE9C4(object->collection_C4, target);
}
