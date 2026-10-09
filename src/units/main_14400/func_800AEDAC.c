#include "common.h"

typedef struct { unsigned char unknown00[0x18]; short adjust18; short unknown1a; void (*method1c)(void *, s32, void *); } VTable;
typedef struct { unsigned char unknown00[0x18]; VTable *field18; } Object;
extern char D_80153A30[], D_80143084[], D_8014303C[], D_80143064[], D_80143044[];
extern void func_800CA4A4(Object *, void *);
void func_800AEDAC(Object *object) {
    VTable *table;
    func_800CA4A4(object, D_80153A30);
    table = object->field18;
    table->method1c((char *)object + table->adjust18, 0xc, D_80143084);
    table = object->field18;
    table->method1c((char *)object + table->adjust18, 1, D_8014303C);
    table = object->field18;
    table->method1c((char *)object + table->adjust18, 0x20, D_80143064);
    table = object->field18;
    table->method1c((char *)object + table->adjust18, 0x20, D_80143044);
}
