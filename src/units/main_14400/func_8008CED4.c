#include "common.h"

typedef struct {
    char pad0[1];
    unsigned char count;
} Ref;

typedef struct {
    char pad0[0x74];
    Ref *ref;
} Obj;

Ref *func_8008CED4(Obj *obj) {
    Ref *ref = obj->ref;

    if (ref != 0) {
        ref->count--;
        obj->ref = 0;
    }
    return ref;
}
