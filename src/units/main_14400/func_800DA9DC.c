#include "common.h"
#include "item_records.h"

typedef struct { char pad0[0x4]; Item *field_4; } Arg;

void func_800DA9DC(unsigned char *out, Arg *arg) {
    *out = func_800AFD08(&D_80143094, arg->field_4);
}
