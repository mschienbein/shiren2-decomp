#include "common.h"

typedef struct { char pad[0x18]; void *unk18; } Obj;
char *func_800AE674(void *);
char *func_800D0038(Obj *obj) {
    return func_800AE674(obj->unk18);
}
