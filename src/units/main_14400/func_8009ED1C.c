#include "common.h"
/* D_8014251C is the +0x4C vtable inside the 0xF4-byte save object. */
typedef struct { unsigned char pad00[0x4C]; const void *vtable; unsigned char pad50[0xA4]; } Object;
extern const unsigned char D_80151E38[144];
extern Object D_801424D0;
void func_8009ED1C(void) { s32 local[4]; D_801424D0.vtable = D_80151E38; }
