#include "common.h"

/* Prefix view of the static widgets constructed by func_8009C920. */
typedef struct {
    unsigned char pad0[0x4C];
    unsigned char *vtable;
} Widget;

extern unsigned char D_80151E38[];
extern Widget D_80141C24;
extern Widget D_80141BC8;

void func_8009C8FC(void)
{
    /* Unused 16-byte local area reserved by the original frame. */
    unsigned char *unused[4];

    D_80141C24.vtable = D_80151E38;
    D_80141BC8.vtable = D_80151E38;
}
