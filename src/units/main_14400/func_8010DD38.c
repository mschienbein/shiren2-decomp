#include "common.h"
typedef struct { unsigned char field_00[0x8]; void *field_08; } Object;
extern char D_80153AA0[];
extern void func_800AC68C(Object *);
void func_8010DD38(Object *object, s32 flags) { object->field_08 = D_80153AA0; if (flags & 1) func_800AC68C(object); }
