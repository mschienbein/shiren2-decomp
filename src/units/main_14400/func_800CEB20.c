#include "common.h"

typedef struct { s32 field_0; void *field_4; s32 field_8; } S;
void func_800CEB54(S *s);
S *func_800CEB20(S *s, void *a) {
    s->field_4 = a;
    s->field_8 = 1;
    func_800CEB54(s);
    return s;
}
