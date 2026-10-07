#include "common.h"

typedef short s16;

typedef struct {
    s16 unk0;
    void *unk4;
} Obj800D9EB8;

extern char D_80157FA8[];
extern char D_80158158[];

Obj800D9EB8 *func_800D9EB8(Obj800D9EB8 *obj) {
    obj->unk4 = D_80157FA8;
    obj->unk0 = 50;
    obj->unk4 = D_80158158;
    return obj;
}
