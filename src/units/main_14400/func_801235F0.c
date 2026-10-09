#include "common.h"

typedef struct { s32 unk0; s32 unk4; void *unk8; } S;
extern char D_8015FC10[];
extern S *func_80116C30(S *, s32);
extern s32 func_80123658(S *);
S *func_801235F0(S *s) {
    func_80116C30(s, 0xCA);
    s->unk8 = D_8015FC10;
    func_80123658(s);
    return s;
}
