#include "common.h"

/* Signed delta at +4 (sh in func_800535D0, lh in func_800536F0); return its low byte. */
typedef struct {
    unsigned char pad00[0x4];
    short delta_04;
} Record_800535C8;

unsigned char func_800535C8(Record_800535C8 *record)
{
    return (unsigned char)record->delta_04;
}
