#include "common.h"
typedef union { u32 value; void *pointer; } Word;
typedef struct { unsigned char pad_0[0x20]; Word field_20; } Object;
Word *func_800A7DFC(Word *result, Object *object) { *result = object->field_20; return result; }
