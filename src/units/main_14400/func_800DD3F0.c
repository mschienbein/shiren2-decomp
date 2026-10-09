#include "common.h"

typedef struct { s32 unk0; const void *vtbl; } Obj;
extern const unsigned char D_801587D8[48];
void *func_800DA8A0(void *obj, s32 kind, void *src);
Obj *func_800DD3F0(Obj *obj, void *src) {
    func_800DA8A0(obj, 0x22, src);
    obj->vtbl = D_801587D8;
    return obj;
}
