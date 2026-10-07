#include "common.h"
typedef struct { char pad[0x1C]; s32 *x1C; } S;
void func_800CABD8(S *p);
void func_800CAD44(S *p);
void func_800CA0A8(s32 *a, s32 b);
void func_800CAF78(S *p) {
    s32 v = *p->x1C;
    func_800CABD8(p);
    func_800CAD44(p);
    func_800CA0A8(p->x1C, v);
}
