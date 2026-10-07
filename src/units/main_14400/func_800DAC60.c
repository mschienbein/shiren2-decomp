#include "common.h"
typedef struct Obj Obj;
typedef struct { char pad[0x20]; short field_20; s32 (*field_24)(void *); char pad28[0x10]; short field_38; Obj *(*field_3C)(void *, u32); } VTable;
struct Obj { unsigned char field_0; VTable *field_4; };
extern Obj *func_8011422C(Obj *);
Obj *func_800DAC60(Obj *a, Obj *b) {
    s32 i;
    if (!a) return 0;
    i = a->field_4->field_24((char *)a + a->field_4->field_20) - 1;
    for (;;) {
        Obj *p;
        if (i < 0) break;
        p = a->field_4->field_3C((char *)a + a->field_4->field_38, i);
        if (p == b) return a;
        if (p->field_0 == 9) { Obj *result = func_800DAC60(func_8011422C(p), b); if (result) return result; }
        i--;
    }
    return 0;
}
