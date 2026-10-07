#include "common.h"
typedef struct { unsigned char field_00[0x16]; unsigned char field_16; unsigned char field_17; } Object;
extern Object D_80165960;
static inline Object *get_object(void) { return &D_80165960; }
void func_8005D8F8(s32 value) { Object *object = get_object(); object->field_17 = 0; object->field_16 = value; }
