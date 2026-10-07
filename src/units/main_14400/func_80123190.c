#include "common.h"

typedef struct { s32 kind_0; } Msg80123190;
void func_80114404(void *obj, s32 a1, s32 a2, s32 a3, s32 a4);
s32 func_80114E28(void *obj, Msg80123190 *msg);

s32 func_80123190(void *obj, Msg80123190 *msg) {
    if (msg->kind_0 == 0x1E) {
        func_80114404(obj, -3, -3, -10, -20);
        return 1;
    }
    return func_80114E28(obj, msg);
}
