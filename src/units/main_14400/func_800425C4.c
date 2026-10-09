#include "common.h"

typedef struct Obj Obj;

void *func_800C9E00(void);
s32 func_800D1A00(Obj *obj, s32 row, s32 col);
s32 func_800D194C(s32 value);

s32 func_800425C4(s32 row, s32 col) {
    return func_800D194C(func_800D1A00(func_800C9E00(), row, col));
}
