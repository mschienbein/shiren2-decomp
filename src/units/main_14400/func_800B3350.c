#include "common.h"
typedef struct { s32 x; s32 y; } Point;
typedef struct { s32 field0; s32 field4; s32 field8; s32 fieldC; } Value;
extern Point *D_801476B8;
/* Returns its rectangle by value through the hidden result pointer. */
extern Value func_800B3024(Point *);
extern s32 func_800A24DC(Point *, Value *);
s32 func_800B3350(Point *p) { Value out; s32 invalid = 0; if (p->y >= 76 || p->x >= 54 || p->y < 0 || p->x < 0) invalid = 1; if (invalid) return 0; out = func_800B3024(p); return func_800A24DC(D_801476B8, &out); }
