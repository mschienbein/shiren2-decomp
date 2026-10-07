#ifndef FIELD_VIEWS_800D0054_H
#define FIELD_VIEWS_800D0054_H

#include "common.h"

/* Compatible partial view for the observed pointer-bearing caller domain. */
typedef struct FieldView800D0054 {
    unsigned char unobserved_00[8];
    unsigned char *pointer_08;
    unsigned char unobserved_0C[4];
    unsigned char bytes_10[8];
    void *owner_18;
} FieldView800D0054;

unsigned char *func_800D0054(FieldView800D0054 *object, void *owner);

#endif
