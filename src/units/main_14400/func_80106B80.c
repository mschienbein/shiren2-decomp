#include "common.h"
typedef struct { unsigned char fields00[0x24]; const void *field24; } Object;
extern const unsigned char D_8015C080[200];
extern void func_800EFD28(Object *object, s32 mode);
extern void func_800A3918(Object *object);
void func_80106B80(Object *object, s32 flags) {
    object->field24 = D_8015C080;
    func_800EFD28(object, 0);
    if (flags & 1) {
        func_800A3918(object);
    }
}
