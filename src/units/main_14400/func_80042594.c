#include "common.h"

void *func_800C9E00(void);
s32 func_800D19C0(unsigned char *p, s32 row);

s32 func_80042594(s32 row) {
    return func_800D19C0(func_800C9E00(), row);
}
