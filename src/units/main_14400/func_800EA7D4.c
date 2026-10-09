#include "common.h"

typedef struct { unsigned char unknown00[0x60]; short adjust60; short unknown62; void (*method64)(void *); } VTable;
typedef struct { unsigned char unknown00[0x24]; VTable *field24; } Object;
extern void func_800E5248(Object *);
void func_800EA7D4(Object *object) {
    VTable *table;
    func_800E5248(object);
    table = object->field24;
    table->method64((char *)object + table->adjust60);
}
