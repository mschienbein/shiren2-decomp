#include "common.h"

typedef struct Obj Obj;
extern s32 func_800E1CC4(Obj *obj, s32 kind);

s32 func_800E303C(Obj *obj) {
    return func_800E1CC4(obj, 4);
}
