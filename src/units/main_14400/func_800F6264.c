#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 w[2]; } Tmp800F6264;
void *func_800B3774(void *out);
s32 func_800A251C(void *x, Tmp800F6264 *y);
s32 func_800F6264(u8 *obj) {
    Tmp800F6264 tmp;
    func_800B3774(&tmp);
    return func_800A251C(obj + 0x8C, &tmp);
}
