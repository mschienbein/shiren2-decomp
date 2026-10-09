#include "common.h"

typedef struct { unsigned char unknown00[0x24]; const void *field24; } Object;
extern const unsigned char D_8015C510[192];
extern void func_800EFD28(Object *, s32);
extern void func_800A3918(Object *);
void func_80108B30(Object *object, s32 flags) {
    object->field24 = D_8015C510;
    func_800EFD28(object, 0);
    if (flags & 1) func_800A3918(object);
}
