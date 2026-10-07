#include "common.h"

typedef struct { char pad0[0x4C]; void *field_4C; } Obj;

extern Obj D_801424D0;
extern char D_80152E00[];
Obj *func_800953C0(Obj *obj);

void func_8009ED38(void) {
    func_800953C0(&D_801424D0);
    D_801424D0.field_4C = D_80152E00;
}
