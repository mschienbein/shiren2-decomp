#include "common.h"
typedef unsigned char u8;
extern void *func_800C9E00(void);
extern s32 func_800D1488(void *self, u8 arg);
s32 func_80042684(s32 value) {
    return func_800D1488(func_800C9E00(), value);
}
