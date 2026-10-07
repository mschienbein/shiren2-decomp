#include "common.h"

typedef struct { char pad0[0xD7]; char unkD7; } Obj;
unsigned char *func_801296C8(Obj *obj, unsigned char *cursor) {
    obj->unkD7 = 1;
    return cursor;
}
