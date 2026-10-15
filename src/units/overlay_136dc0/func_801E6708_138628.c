#include "func_801E6708_138628/types.h"

void func_801E6708_138628(s32 index, s32 sub, s32 value, s32 kind)
{
    s32 x, y;
    func_8004194C(index, &x, &y);
    func_80075F44(kind, 0, sub);
    func_800748F8(index, kind, x, y, value, 1);
    if (sub) {
        HostUnit *p;
        HostFrame *frames;
        s32 count;
        func_80075E5C(index, 5);
        p = func_8007946C(0, index);
        frames = D_8014D1B4[func_80074784(kind, 0)->metadata->b03].frames;
        count = 0;
        while (frames[count].id != -1) count++;
        count--;
        if (count > 0) {
            p->b42 = func_80042498((u8)count);
            p->b43 = func_80042498(8);
        }
    } else {
        func_80075E5C(index, 0);
    }
    (D_801F6764_148684 + index)->b15 = 1;
}
