#include "common.h"

extern const unsigned char D_80151E38[144];
/* The widget occupies D_80140470..D_801404DF. */
typedef struct Widget70 { char pad0[0x4C]; const void *field_4C; char pad50[0x20]; } Widget70;
extern Widget70 D_80140470;
void func_800979C4(void) { char buf[0x10]; D_80140470.field_4C = D_80151E38; }
