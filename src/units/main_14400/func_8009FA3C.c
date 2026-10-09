#include "common.h"
typedef struct { unsigned char field_00[0x100]; unsigned char field_100[0x10]; } Object;
extern void func_80048728(void *);
void func_8009FA3C(Object *object) { func_80048728(object->field_100); }
