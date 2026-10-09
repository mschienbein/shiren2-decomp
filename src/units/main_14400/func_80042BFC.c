#include "common.h"

typedef unsigned char u8;

/* 0x20-byte reader object; vtable at +0, cached index at +0x10. */
typedef struct {
    const void *vtable;
    u8 pad4[0xC];
    s32 x10;
    u8 pad14[0xC];
} Reader;

extern Reader D_80138AE0;
extern const unsigned char D_80151DF8[24];

/* Static constructor (ctor list entry) for the global reader D_80138AE0. */
void func_80042BFC(void)
{
    Reader *reader = &D_80138AE0;

    reader->vtable = D_80151DF8;
    reader->x10 = -1;
}
