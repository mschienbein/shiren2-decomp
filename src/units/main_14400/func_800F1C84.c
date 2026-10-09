#include "common.h"
typedef unsigned short u16;
typedef struct { unsigned char pad_00[0x94]; u16 name_94; } Object800F1C84;
char *func_80048480(u16 id);
char *func_80083C90(char *destination, char *source);
char *func_800F1C84(Object800F1C84 *object, char *destination)
{
    func_80083C90(destination, func_80048480(object->name_94));
    return destination;
}
