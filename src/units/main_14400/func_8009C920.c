#include "common.h"
/* Complete static widgets: D_80141BC8 is 0x5C bytes; D_80141C24 is 0x10C. */
typedef struct { unsigned char pad00[0x4C]; const void *vtable; unsigned char pad50[0xC]; } Base;
typedef struct { Base base; unsigned char pad5C[0xC]; s32 selection; const void *data; unsigned char pad70[0x9C]; } Object;
extern Base D_80141BC8;
extern Object D_80141C24;
extern const unsigned char D_801521D0[144], D_80152AE8[144], D_80151EC8[];
extern Base *func_800953C0(Base *object);
void func_8009C920(void) {
    func_800953C0(&D_80141BC8);
    D_80141BC8.vtable = D_801521D0;
    func_800953C0(&D_80141C24.base);
    D_80141C24.base.vtable = D_80152AE8;
    D_80141C24.selection = -1;
    D_80141C24.data = D_80151EC8;
}
