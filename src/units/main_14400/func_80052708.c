#include "common.h"

typedef unsigned char u8;
typedef short s16;
extern u8 D_8016166E;
void func_80053184(s16 a, u8 b, u8 c);
void func_80052708(s16 a, u8 b, u8 c) {
    if (D_8016166E == 0) {
        func_80053184(a, b, c);
    }
}
