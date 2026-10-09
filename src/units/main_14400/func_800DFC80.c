#include "common.h"

typedef struct {
    short field_00;
    short field_02;
    void *table_04;
} Object;

extern s32 D_80157FA8[];
extern s32 D_80158B38[];

Object *func_800DFC80(Object *object) {
    object->table_04 = D_80157FA8;
    object->field_00 = 0x30;
    object->table_04 = D_80158B38;
    return object;
}
