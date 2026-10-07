#include "common.h"

typedef struct {
    short field_0;
    void *field_4;
} Obj;

extern char D_80157FA8[];
extern char D_80157FD8[];

Obj *func_800D90E8(Obj *obj) {
    obj->field_4 = D_80157FA8;
    obj->field_0 = 5;
    obj->field_4 = D_80157FD8;
    return obj;
}
