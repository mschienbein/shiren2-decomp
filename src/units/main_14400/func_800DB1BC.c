#include "common.h"

typedef struct {
    s32 unk0;
    void *vtable;
} Obj800DB1BC;

extern char D_80158448[];
extern void *func_800DA904(void *obj, s32 kind, unsigned char *params);

Obj800DB1BC *func_800DB1BC(Obj800DB1BC *obj, unsigned char *arg1) {
    func_800DA904(obj, 15, arg1);
    obj->vtable = D_80158448;
    return obj;
}
