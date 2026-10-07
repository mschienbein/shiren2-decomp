#include "common.h"

typedef unsigned char u8;
extern u8 D_80157FA8[];
void func_800D03C4(void *, s32);
void func_800D8FE8(void *);

void func_800DE09C(u8 *obj, s32 flags)
{
    u8 *member = obj + 0xB0;
    u8 *begin;
    u8 *p;

    func_800D03C4(member, 2);
    begin = obj + 8;
    if (begin != 0) {
        p = member;
        while (begin != p) {
            p -= 8;
        }
    }
    *(void **)(obj + 4) = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
