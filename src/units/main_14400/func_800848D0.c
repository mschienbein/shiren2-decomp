#include "common.h"

typedef struct {
    char pad0[4];
    short unk4;
} Obj800848D0;

void func_80042920(void);
void func_8007D8A0(s32 arg0);

void func_800848D0(Obj800848D0 *obj) {
    func_80042920();
    func_8007D8A0(0);
    obj->unk4 = 4;
}
