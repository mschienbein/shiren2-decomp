#include "common.h"

extern s32 D_80157FA8[];
extern void func_800D8FE8(void *object);

typedef struct {
    unsigned short field_0;
    s32 *field_4;
} Obj_800DA1F4;

void func_800DA1F4(Obj_800DA1F4 *obj, s32 flags) {
    obj->field_4 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
