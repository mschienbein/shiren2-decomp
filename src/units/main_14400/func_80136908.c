#include "common.h"

/* Partial view: only the field cleared here is known. */
typedef struct {
    u32 field00;
} Record_80136908;

void func_80136908(Record_80136908 *record)
{
    record->field00 = 0;
}
