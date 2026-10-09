#include "common.h"

typedef unsigned char u8;
typedef struct Obj800A38A0 Obj800A38A0;
extern void func_800F479C(u8 *self, s32 flags);
extern void func_800A3918(Obj800A38A0 *obj);
void func_800F5B54(u8 *self, s32 flags)
{
    func_800F479C(self, 0);
    if (flags & 1) {
        func_800A3918((Obj800A38A0 *)self);
    }
}
