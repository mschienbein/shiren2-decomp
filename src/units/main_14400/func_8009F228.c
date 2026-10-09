#include "common.h"

/* D_801426FC is the +0x4C vtable of the complete 0x1E4-byte item picker. */
typedef struct { unsigned char pad00[0x4C]; const void *vtable; unsigned char pad50[0x194]; } Object;
extern const unsigned char D_80151E38[144];
extern Object D_801426B0;
void func_8009F228(void) { unsigned char frame[16]; D_801426B0.vtable = D_80151E38; }
