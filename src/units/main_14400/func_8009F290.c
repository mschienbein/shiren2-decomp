#include "common.h"
extern s32 D_80151E38[];
extern void func_800D8FA8(void *object);
typedef struct { unsigned char pad_00[0x4C]; s32 *field_4C; } Object;
void func_8009F290(Object *object, s32 flags) {
    object->field_4C = D_80151E38;
    if (flags & 1) func_800D8FA8(object);
}
