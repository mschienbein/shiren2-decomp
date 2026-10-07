#include "common.h"

extern void *D_801476B8;
s32 func_800CD090(void *container, void *element); void *func_800EB9FC(void *obj); char *func_800AE674(void *obj); void func_800498E4(s32 message_id, ...);
s32 func_800CFFC8(void *a, void *b, s32 c){
    if (func_800CD090(a, b) < 0) return 0;
    if (func_800EB9FC(D_801476B8) == 0) return 1;
    if (c) func_800498E4(0xB6, func_800AE674(b));
    return 0;
}
