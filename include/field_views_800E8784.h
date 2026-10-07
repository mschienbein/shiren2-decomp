#ifndef FIELD_VIEWS_800E8784_H
#define FIELD_VIEWS_800E8784_H

#include "common.h"

/* Compatible partial word view; historical full object and field types unknown. */
typedef struct FieldView800E8784 {
    unsigned char unobserved_00[0x78];
    u32 word_78;
    u32 word_7C;
} FieldView800E8784;

void func_800E8784(FieldView800E8784 *object);

#endif
