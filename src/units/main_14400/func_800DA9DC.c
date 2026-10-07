#include "common.h"

typedef struct { char pad0[0x4]; void *field_4; } Arg;

extern char D_80143094[];
s32 func_800AFD08(void *table, void *obj);

void func_800DA9DC(unsigned char *out, Arg *arg) {
    *out = func_800AFD08(D_80143094, arg->field_4);
}
