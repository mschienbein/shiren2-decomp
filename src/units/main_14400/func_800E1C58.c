#include "common.h"

typedef struct { char pad[0x43]; unsigned char unk43; } S;
extern s32 func_800E1CC4(S *, s32);
extern s32 func_800E1CD4(S *, s32);
extern s32 func_800E1D14(S *, s32);
s32 func_800E1C58(S *s, s32 kind) {
    if (kind < 10) return func_800E1CC4(s, kind);
    if (kind < 17) return func_800E1CD4(s, kind);
    if (kind < 19) return s->unk43 != 0;
    if (kind < 21) return func_800E1D14(s, kind);
    return 0;
}
