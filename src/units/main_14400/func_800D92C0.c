#include "common.h"

typedef unsigned short u16;

typedef struct {
    u16 kind;
    u16 pad2;
    void *data;
} Obj800D92C0;

extern s32 D_80157FA8[];
extern s32 D_80158008[];

Obj800D92C0 *func_800D92C0(Obj800D92C0 *obj) {
    obj->data = D_80157FA8;
    obj->kind = 6;
    obj->data = D_80158008;
    return obj;
}
