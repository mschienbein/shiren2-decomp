#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad0[10]; u8 field_A; } Object;
s32 func_800EE58C(u8 value);
char *func_80048480(u16 id);
char *func_80083C90(char *dst, char *src);
char *func_800EEA04(Object *self, char *out) {
    func_80083C90(out, func_80048480(func_800EE58C(self->field_A)));
    return out;
}
