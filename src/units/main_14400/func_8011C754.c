#include "common.h"
typedef struct { unsigned char pad_0[8]; const void *field_8; } Object;
extern const unsigned char D_80153AA0[];
extern void func_800AC68C(void *);
void func_8011C754(Object *object, s32 flags) { object->field_8 = D_80153AA0; if (flags & 1) func_800AC68C(object); }
