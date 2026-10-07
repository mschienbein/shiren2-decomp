#include "common.h"

/* Owned 4-byte .rodata name text at 0x801544BC: Shift-JIS half-width katakana
 * BE B2, NUL, zero fill. func_800CFE90 is the name slot (+0x74) of D_801544C0;
 * like the other name slots (func_800CECEC, func_800D0038) it returns char *. */
const char D_801544BC[4] = "\xBE\xB2";

/* The name-slot receiver is passed by the vtable contract but unused here. */
char *func_800CFE90(void *self) { return (char *)D_801544BC; }
