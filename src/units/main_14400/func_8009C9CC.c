#include "common.h"

typedef struct {
    char pad0[0x4C];
    void *vtable4C;
    char pad50[0xA8 - 0x50];
    void *vtableA8;
} Obj8009C9CC;

extern char D_80152968[];
extern char D_80151E38[];
extern void func_800D8FA8(void *object);

void func_8009C9CC(Obj8009C9CC *obj, s32 flags) {
    obj->vtable4C = D_80152968;
    obj->vtableA8 = D_80151E38;
    obj->vtable4C = D_80151E38;
    if (flags & 1) {
        func_800D8FA8(obj);
    }
}
