#include "common.h"
typedef struct { unsigned char pad_0[8]; const void *vtable_8; } Object;
extern const unsigned char D_80153AA0[];
extern void func_800AC68C(void *a);
void func_8011B900(Object *p, s32 flags) { p->vtable_8 = D_80153AA0; if (flags & 1) func_800AC68C(p); }
