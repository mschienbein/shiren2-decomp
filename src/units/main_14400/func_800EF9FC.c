#include "common.h"
typedef unsigned short u16;
typedef struct { unsigned char field_00[10]; unsigned char field_0A; } Object;
extern char *func_80048480(u16 id);
extern char *func_80083C90(char *dst, char *src);
char *func_800EF9FC(Object *object, char *destination) {
    func_80083C90(destination, func_80048480(object->field_0A + 0x26B2));
    return destination;
}
