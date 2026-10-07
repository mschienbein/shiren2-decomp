#include "func_80055C00_types.h"

/* Table entry at original ROM 0x10CB5C / RAM 0x80139B1C.
 * This writes twelve fields, leaving the rest of the record unchanged.
 */
void func_80055C00(Record_801D40DC *record)
{
    record->field_14 = 0xFF;
    record->field_15 = 0xFF;
    record->field_16 = 0xFF;
    record->field_18 = 0xFF;
    record->field_19 = 0xFF;
    record->field_1A = 0xFF;
    record->field_1B = 0xFF;
    record->field_5E = 1;
    record->field_5C = 1;
    record->field_17 = 0;
    record->field_50 = 2;
    record->field_5A = 0;
}
