#include "common.h"
/* D_8014263C is the +0x4C vtable inside the 0x5C-byte picker object. */
typedef struct { unsigned char pad00[0x4C]; const void *vtable; unsigned char pad50[0xC]; } Object;
extern Object D_801425F0;
extern const unsigned char D_80151E38[144];
void func_8009EE94(void) { s32 local[4]; D_801425F0.vtable = D_80151E38; }
