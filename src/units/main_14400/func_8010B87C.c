#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { void *field_0; void *field_4; } Obj8010B87C;
void *func_8010B5D0(void *value);

void *func_8010B87C(Obj8010B87C *obj) {
    return func_8010B5D0(obj->field_4);
}
