#include "common.h"

typedef struct { unsigned char field00; unsigned char unknown01[11]; void *field0c; } Object;
extern void func_80091544(void *);
void func_8008DEC0(Object *object) {
    if (object->field00) {
        void *resource = object->field0c;
        object->field00 = 0;
        if (resource) { func_80091544(resource); object->field0c = 0; }
    }
}
