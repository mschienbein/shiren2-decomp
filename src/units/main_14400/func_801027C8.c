#include "common.h"
extern s32 D_8015B838[];
extern void func_800EFD28(void *object, s32 flags);
extern void func_800A3918(void *object);
typedef struct { unsigned char pad_00[0x24]; s32 *field_24; } Object;
void func_801027C8(Object *object, s32 flags) {
    object->field_24 = D_8015B838;
    func_800EFD28(object, 0);
    if (flags & 1) func_800A3918(object);
}
