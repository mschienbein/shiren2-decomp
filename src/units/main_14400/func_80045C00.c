#include "common.h"

typedef unsigned char u8;
typedef short s16;

extern void func_80052708(s16 a, u8 b, u8 c);

/* Only the first operand is word-sized; ROM 0x18C44..0x18C48 narrows it. */
void func_80045C00(s32 a, u8 b, u8 c) {
    s16 id = a;
    func_80052708(id, b, c);
}
