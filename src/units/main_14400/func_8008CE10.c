#include "common.h"

typedef unsigned char u8;
typedef float f32;

f32 func_8008CE10(u8 *self) {
    f32 *p = *(f32 **)(*(u8 **)(self + 0x74) + 0xC);
    if (p != 0) {
        return *p;
    }
    return 0.0f;
}
