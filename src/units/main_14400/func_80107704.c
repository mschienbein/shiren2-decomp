#include "common.h"

typedef struct {
    s32 fields_00[9];
    void *table_24;
} Object;

extern s32 D_8015C2C8[];
extern void func_800EFD28(Object *, s32);
extern void func_800A3918(Object *);

void func_80107704(Object *object, s32 flags) {
    object->table_24 = D_8015C2C8;
    func_800EFD28(object, 0);
    if (flags & 1) {
        func_800A3918(object);
    }
}
