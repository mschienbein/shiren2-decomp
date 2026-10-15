#include "common.h"
#include "item_records.h"

typedef struct { char pad[0x18]; Item *unk18; } Obj;
char *func_800D0038(Obj *obj) {
    return func_800AE674(obj->unk18);
}
