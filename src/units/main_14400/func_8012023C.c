#include "common.h"

typedef struct {
    char pad0[0x8];
    void *vtable;
} Obj8012023C;

extern char D_80153AA0[];
extern void func_800AC68C(Obj8012023C *obj);

void func_8012023C(Obj8012023C *obj, s32 flags) {
    obj->vtable = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
