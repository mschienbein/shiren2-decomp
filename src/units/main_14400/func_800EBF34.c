#include "common.h"

typedef struct { unsigned char unknown00[0x108]; unsigned char field108; } Object;
extern unsigned short D_8014767C;
extern void func_800EBF94(Object *);
extern char *func_800A3B20(Object *);
extern void func_800498E4(s32, ...);
void func_800EBF34(Object *object) {
    if (object->field108) {
        object->field108 = 0;
        if (D_8014767C & 3) {
            func_800EBF94(object);
            func_800498E4(0x1a7, func_800A3B20(object));
        }
    }
}
