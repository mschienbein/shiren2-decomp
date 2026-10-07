#include "common.h"

typedef struct { s32 unk0; s32 unk4; } Pair;
s32 func_800ADF20(void *obj, void *avoid);
void func_800AF558(void *obj) {
    Pair pair;
    Pair *p = &pair;
    pair.unk0 = 0;
    p->unk4 = 0;
    func_800ADF20(obj, p);
}
