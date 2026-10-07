#ifndef SHIREN2_PROBE_80057F60_FIELDS_H
#define SHIREN2_PROBE_80057F60_FIELDS_H

#include "common.h"

/* Partial view of bytes accessed at caller record+0x14. Names are offsets;
 * this is not a declaration of the complete allocation or its game meaning. */
typedef struct Fields80057F60 {
    u32 word00;
    u32 untouched04;
    unsigned char bytes08[16];
} Fields80057F60;

#endif
