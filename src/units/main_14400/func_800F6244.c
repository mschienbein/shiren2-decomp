#include "common.h"

typedef struct Query Query;
typedef struct {
    unsigned char pad_00[0x80];
    Query *field_80;
} Obj;
extern s32 func_800D2FB0(Query *q);

s32 func_800F6244(Obj *obj) {
    return func_800D2FB0(obj->field_80) > 0;
}
