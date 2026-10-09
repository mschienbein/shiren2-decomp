#include "common.h"

/* Complete static widgets: base 0x5C bytes, extended menu 0x10C bytes. */
typedef struct {
    unsigned char pad0[0x4C];
    const void *vtable;
    unsigned char pad50[0xC];
} Widget;
typedef struct {
    Widget base;
    unsigned char pad5C[0xB0];
} ExtendedWidget;

extern const unsigned char D_80151E38[144];
extern ExtendedWidget D_80141C24;
extern Widget D_80141BC8;

void func_8009C8FC(void)
{
    /* Unused 16-byte local area reserved by the original frame. */
    unsigned char *unused[4];

    D_80141C24.base.vtable = D_80151E38;
    D_80141BC8.vtable = D_80151E38;
}
