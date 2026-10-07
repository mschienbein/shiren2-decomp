#include "common.h"
typedef struct { unsigned char field_00[0x24]; void *field_24; } Object;
extern char D_8015B068[];
extern void func_800EFD28(Object *, s32);
extern void func_800A3918(Object *);
void func_800FFFBC(Object *object, s32 flags) { object->field_24 = D_8015B068; func_800EFD28(object, 0); if (flags & 1) func_800A3918(object); }
