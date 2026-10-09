#include "common.h"

/* Complete 0xF4-byte menu, ending before D_801425C4. */
typedef struct { char pad0[0x4C]; void *field_4C; char pad50[0xA4]; } Obj;

extern Obj D_801424D0;
extern char D_80152E00[];
Obj *func_800953C0(Obj *obj);

void func_8009ED38(void) {
    func_800953C0(&D_801424D0);
    D_801424D0.field_4C = D_80152E00;
}
