#include "common.h"

typedef unsigned char u8;

typedef struct { u8 kind_0; } Obj800AE648;
void func_801153FC(Obj800AE648 *obj);

void func_800AE648(Obj800AE648 *obj) {
    if (obj->kind_0 == 9) {
        func_801153FC(obj);
    }
}
