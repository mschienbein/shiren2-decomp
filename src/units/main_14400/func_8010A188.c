#include "common.h"
typedef struct List800ECB5C List800ECB5C;
typedef struct Obj800ECB5C Obj800ECB5C;
typedef struct { unsigned char pad_0[0xC4]; unsigned char field_C4[0x10]; } Object;
extern void func_800EE97C(Object *, Obj800ECB5C *);
extern void func_800CE918(List800ECB5C *, Obj800ECB5C *);
void func_8010A188(Object *object, Obj800ECB5C *item) { func_800EE97C(object, item); func_800CE918((List800ECB5C *)object->field_C4, item); }
