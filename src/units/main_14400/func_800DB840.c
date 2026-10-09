#include "common.h"
typedef struct { unsigned char pad_0[4]; const void *field_4; } Object;
extern const unsigned char D_80157FA8[];
extern void func_800D8FE8(void *);
void func_800DB840(Object *object, s32 flags) { object->field_4 = D_80157FA8; if (flags & 1) func_800D8FE8(object); }
