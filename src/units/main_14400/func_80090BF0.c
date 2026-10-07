#include "common.h"

/* Partial view of the animated record through its float at 0x188. */
typedef struct {
    unsigned char pad00[0x188];
    float field188;
} Record_80090BF0;

void func_80090BF0(Record_80090BF0 *record, float value)
{
    record->field188 = value;
}
