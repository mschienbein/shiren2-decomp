#ifndef FIELD_VIEWS_80123658_H
#define FIELD_VIEWS_80123658_H

#include "common.h"

/* Minimum observed span, not a complete historical object declaration. */
typedef struct {
    unsigned char unknown_00[0x10];
    unsigned short half_10;
    unsigned char unknown_12[2];
    s32 word_14;
} FieldView_80123658;

#endif
