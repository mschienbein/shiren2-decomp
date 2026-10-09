#include "common.h"
typedef unsigned char u8;
typedef struct { s32 a, b; } Pair;
typedef struct { char pad0[0x48]; s32 v48; char pad4C[0x10]; char v5C[0x6C]; s32 vC8; } S;
/* Complete 16-byte menu layout record (.data 0x80138EF8..0x80138F07, {1, 15, 5, 4}): passed to
 * func_80097240 as the layout, whose func_8009543C reads +0, +6, +8 and +0xC. */
typedef struct { s32 columns, width, x, y; } Layout80096140;
extern Layout80096140 D_80138EF8;
extern void func_8009CA80(S *);
extern void func_8009CC34(S *);
extern u8 *func_8006A810(void *, s32, s32);
extern void func_80097240(S *, void *, Layout80096140 *, Pair *);
void func_8009CDD8(S *p) {
    Pair copy;
    Pair tmp;
    func_8009CA80(p);
    func_8009CC34(p);
    D_80138EF8.columns = p->vC8;
    func_8006A810(&tmp, 0, 8);
    tmp.a = p->vC8;
    copy = tmp;
    func_80097240(p, p->v5C, &D_80138EF8, &copy);
    p->v48 = 60000000;
}
