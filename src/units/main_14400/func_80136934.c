#include "common.h"


/* Opaque mixed adjustment/function-pointer table; only its address is used here. */
extern const unsigned char D_80151E38[144];
extern void func_800D8FA8(void *object);

typedef struct {
    char pad0[0x4C];
    const void *vtbl_4C;
} Obj_80136934;

void func_80136934(Obj_80136934 *obj, s32 flags) {
    obj->vtbl_4C = D_80151E38;
    if (flags & 1) {
        func_800D8FA8(obj);
    }
}
