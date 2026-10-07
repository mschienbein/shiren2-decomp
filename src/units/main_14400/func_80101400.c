#include "common.h"
typedef struct { char pad[0x24]; void *f24; } SA;
extern char D_8015B500[];
void func_800EFD28(SA *p, s32 v);
void func_800A3918(SA *p);
void func_80101400(SA *p, s32 flags) {
    p->f24 = D_8015B500;
    func_800EFD28(p, 0);
    if (flags & 1) {
        func_800A3918(p);
    }
}
