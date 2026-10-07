#include "common.h"

typedef struct {
    char pad0[0x8];
    char field_8[1];
} Obj;

char *func_800A7DE4(Obj *obj) {
    return obj->field_8;
}
