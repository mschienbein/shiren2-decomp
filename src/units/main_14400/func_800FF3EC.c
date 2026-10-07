#include "common.h"

extern s32 D_8015AD68;
typedef struct { char pad[0x24]; s32 *vt24; } S;
void func_800EFD28(S *p, s32 arg);
void func_800A3918(S *p);
void func_800FF3EC(S *p, s32 flags) {
    p->vt24 = &D_8015AD68;
    func_800EFD28(p, 0);
    if (flags & 1) func_800A3918(p);
}
