#include "common.h"
typedef struct { s32 f0; s32 f4; } Pair;
s32 func_80111A20(void *a, void *b);
void *func_800A6CC0(void *out_position, void *obj);
void func_8011F560(void *a, void *b, Pair *t, void *c);
void func_8011F4F0(void *a, void *b) {
    Pair tmp;
    if (func_80111A20(a, b) != 0) {
        func_800A6CC0(&tmp, b);
        func_8011F560(a, b, &tmp, (char *)b + 8);
    }
}
