#include "common.h"

typedef short s16;
typedef struct { char pad0[0xC]; s16 field_C; } Obj;

void func_8010CC38(Obj *o, s16 v) {
    o->field_C = v;
}
