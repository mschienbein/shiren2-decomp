#include "common.h"

typedef struct VTable VTable;
typedef struct {
    s32 fields_00[2];
    const VTable *table_08;
} Object;

extern const VTable D_801607B8;
extern void *func_80117230(Object *, s32);

Object *func_80128F50(Object *object) {
    func_80117230(object, 0xF3);
    object->table_08 = &D_801607B8;
    return object;
}
