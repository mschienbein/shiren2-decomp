#include "common.h"

typedef struct {
    unsigned char pad_00[0x74];
    void *field_74;
} Obj;
extern char *func_800AE674(void *obj);

char *func_80096E0C(Obj *obj) {
    return func_800AE674(obj->field_74);
}
