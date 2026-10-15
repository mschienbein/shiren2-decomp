#include "common.h"
#include "item_records.h"

typedef struct {
    unsigned char pad_00[0x74];
    Item *field_74;
} Obj;

char *func_80096E0C(Obj *obj) {
    return func_800AE674(obj->field_74);
}
