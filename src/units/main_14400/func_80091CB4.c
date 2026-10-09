#include "common.h"

typedef struct {
    s32 fields_00[18];
    short adjust_48;
    short field_4A;
    s32 (*method_4C)(void *);
} MethodTable;

typedef struct {
    s32 fields_00[3];
    MethodTable *table_0C;
    s32 field_10;
} Object;

extern s32 func_80091A1C(Object *, s32);
extern void func_80091D20(Object *, u32);

s32 func_80091CB4(Object *object, s32 direction) {
    s32 result = 0;
    if (object->field_10 != 0) {
        result = func_80091A1C(object, direction);
        if (result != 0) {
            MethodTable *table = object->table_0C;
            func_80091D20(object, (u32)table->method_4C((unsigned char *)object + table->adjust_48));
        }
    }
    return result;
}
