#include "common.h"
/* Stream vtable slot +0x28/+0x2C reads `size` bytes into `data` (void, e.g. func_800CA668). */
typedef struct { unsigned char pad[0x28]; short adjust_28; unsigned short reserved_2A; void (*read_2C)(void *self, s32 size, void *data); } VTable;
/* Stream object: +0x14 holds the failing tag/error text pointer (null when clean). */
typedef struct { unsigned char pad[0x14]; const char *field14; VTable *field18; } Object;
extern Object *D_80147F44;
extern unsigned char D_80154254[];
extern void func_800CA0A8(Object *, s32);
extern s32 func_80083D8C(void *, void *, u32);
s32 func_800CBE44(void) {
    unsigned char buffer[4];
    Object *obj;
    func_800CA0A8(D_80147F44, 8);
    obj = D_80147F44;
    obj->field18->read_2C((unsigned char *)obj + obj->field18->adjust_28, 4, buffer);
    return !D_80147F44->field14 && !func_80083D8C(buffer, D_80154254, 4);
}
