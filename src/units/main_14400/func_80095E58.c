#include "common.h"
typedef struct { s32 a; s32 b; } Pair800489E0;
typedef struct Obj800489E0 Obj800489E0;
typedef struct { s32 field_0; Obj800489E0 *field_4; Pair800489E0 field_8; } Dst;
void func_80095E58(void *dst, Obj800489E0 *obj, Pair800489E0 pair) {
    Dst *p = dst;
    p->field_4 = obj;
    p->field_8 = pair;
}
