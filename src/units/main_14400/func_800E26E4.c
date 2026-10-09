#include "common.h"

/* Slot +0x94 targets func_800E115C / derived dispatchers; +0x90 is zero in their tables. */
typedef struct {
    s32 fields_00[36];
    short adjust_90;
    short field_92;
    s32 (*method_94)(void *, s32, s32, unsigned char, s32);
} MethodTable;

typedef struct {
    s32 fields_00[9];
    MethodTable *table_24;
} Object;

void func_800E26E4(Object *object) {
    MethodTable *table = object->table_24;
    table->method_94((unsigned char *)object + table->adjust_90, 0, 0x14, 0xFE, 0);
}
