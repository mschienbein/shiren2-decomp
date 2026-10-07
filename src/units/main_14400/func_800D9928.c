#include "common.h"
typedef struct { s32 field_0; void *vtable; } Obj800D9928;
extern s32 D_80158098;
void func_800DDAD0(Obj800D9928 *obj, s32 kind);
void func_800DDC0C(Obj800D9928 *obj, unsigned char *data, s32 len);
Obj800D9928 *func_800D9928(Obj800D9928 *obj, unsigned char *data) {
    s32 len;
    func_800DDAD0(obj, 9);
    obj->vtable = &D_80158098;
    len = *data++;
    func_800DDC0C(obj, data, len - 1);
    return obj;
}
