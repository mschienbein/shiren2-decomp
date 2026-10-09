#ifndef SHIREN2_OBSERVED_80058BA8_FIELDS_H
#define SHIREN2_OBSERVED_80058BA8_FIELDS_H

#include "common.h"

typedef unsigned short u16;
typedef unsigned char u8;

/* Complete twelve-byte raw input record (func_800585B0 clears 0xC bytes from
 * D_80163118; func_80058680 reads/writes the halfword at +0xA from the same base),
 * also consumed by func_80058C00. */
typedef struct { u16 buttons_00; u8 x_02, y_03; u16 field_04, buttons_06, buttons_08, previous_0A; } Raw;
extern Raw D_80163118;

#endif
