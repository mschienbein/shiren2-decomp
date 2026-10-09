#include "common.h"

typedef struct { unsigned char unknown00[0xd]; signed char field0d; } Result;
typedef struct { unsigned char unknown00[0x38]; short adjust38; short unknown3a; Result *(*method3c)(void *, u32); } VTable;
typedef struct { s32 unknown00; VTable *field04; } Child;
typedef struct { Child *field00; Result *field04; } Object;
typedef struct { s32 field00; s32 unknown04[3]; } Query;
extern s32 func_8009A2C8(void *);
extern s32 func_800A0E1C(Object *, Query *, unsigned short, s32 (*)(void *));
s32 func_800A0F94(Object *object) {
    Query query;
    s32 status = func_800A0E1C(object, &query, 0x1e9, func_8009A2C8);
    Result *result;
    Child *child;
    VTable *table;
    if (status != 1) return status;
    child = object->field00;
    table = child->field04;
    result = table->method3c((char *)child + table->adjust38, query.field00);
    object->field04 = result;
    if (result->field0d >= 0x63) return -2;
    return 1;
}
