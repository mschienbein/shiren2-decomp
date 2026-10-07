#include "common.h"

/* Partial byte view; the complete record and historical API are unresolved. */
void func_800CABF8(unsigned char *record, u32 first, u32 second)
{
    record[0x0B] = (unsigned char)first;
    record[0x0C] = (unsigned char)second;
}
