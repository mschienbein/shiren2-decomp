#include "common.h"
typedef struct { s32 fields[4]; } Value;
/* Owner slot 0x3C returns the selected item pointer (same slot as func_800A18AC). */
typedef struct { unsigned char pad_00[0x38]; short adjust_38; short pad_3A; void *(*method_3C)(void *, u32); } VTable;
typedef struct { s32 field_00; const VTable *field_04; } Child;
typedef struct { Child *field_00; void *field_04; } Object;
extern s32 func_8009A330(void *object);
extern s32 func_800A0E1C(Object *object, Value *value, unsigned short mode, s32 (*callback)(void *));
s32 func_800A1100(Object *object) {
    Value value;
    s32 result = func_800A0E1C(object, &value, 0x1EC, func_8009A330);
    if (result != 1) return result;
    {
        Child *child = object->field_00;
        object->field_04 = child->field_04->method_3C((char *)child + child->field_04->adjust_38, value.fields[0]);
    }
    return 1;
}
