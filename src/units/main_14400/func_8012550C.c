#include "common.h"
typedef short s16;
typedef struct { s16 delta; s16 index; void (*fn)(void *, s32); } VtblEntry;
typedef unsigned char u8;
typedef struct { char pad[0x8]; VtblEntry *vtbl; } Obj8012550C;
extern u8 D_80156A79;
s32 func_80049CB4(s32, ...);
void func_80115E18(void *, void *);
void func_800A00C4(void *, u8, void *, s32);
/* Trap slot +0x44 supplies arg4 (direction) and arg5 (target unit), unused here. */
s32 func_8012550C(void *arg0, void *arg1, void *arg2, void *arg3, void *arg4, void *arg5, Obj8012550C *arg6) {
    func_80049CB4(0xF6, arg3);
    func_80115E18(arg0, arg2);
    if (arg6 != 0) {
        func_80049CB4(0xCD, arg6, arg3);
    }
    func_800A00C4(arg3, D_80156A79, arg1, 9);
    if (arg6 == 0) {
        return 1;
    }
    {
        VtblEntry *e = &arg6->vtbl[1];
        e->fn((char *)arg6 + e->delta, 3);
    }
    return 0;
}
