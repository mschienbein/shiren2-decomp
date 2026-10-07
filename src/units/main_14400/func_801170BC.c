#include "common.h"
typedef unsigned short u16;

extern char *func_80048480(u16 id);
extern char *func_80083C90(char *destination, char *source);
char *func_801170BC(char *destination, s32 textId) {
    return func_80083C90(destination, func_80048480((u16)((u32)textId + 0x20Cu)));
}
