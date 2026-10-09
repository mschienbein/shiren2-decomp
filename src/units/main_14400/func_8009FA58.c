#include "common.h"
typedef struct { s32 x, y; void *target; s32 handle; } Display;
typedef struct { unsigned char fields00[0x100]; Display display100; unsigned char callback110[0x10]; unsigned short field120; } Object;
extern char *func_80048480(unsigned short value);
extern void func_800487EC(void *object, s32 x, s32 y, const char *text);
void func_8009FA58(Object *object) {
    func_800487EC(&object->display100, 0, 0, func_80048480(object->field120));
}
