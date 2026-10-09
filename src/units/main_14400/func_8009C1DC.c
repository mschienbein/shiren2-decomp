#include "common.h"
/* The former D_80141B1C label is the +0x4C vtable of this 0x8C-byte object. */
typedef struct { unsigned char pad00[0x4C]; const void *vtable; unsigned char pad50[0x3C]; } Object;
extern const unsigned char D_80151E38[144];
extern Object D_80141AD0;
void func_8009C1DC(void) { s32 local[4]; D_80141AD0.vtable = D_80151E38; }
