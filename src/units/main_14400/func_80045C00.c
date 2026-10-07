#include "common.h"

typedef unsigned char u8;
typedef short s16;

extern void func_80052708(s16 a, u8 b, u8 c);

void func_80045C00(s16 a, u8 b, u8 c) {
    func_80052708(a, b, c);
}
