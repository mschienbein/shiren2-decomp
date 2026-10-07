#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 value; u8 pad[0xF]; } Buf800A991C;
void func_800ABB50(Buf800A991C *out, u8 a, u8 b);
u8 func_800A991C(u8 a, u8 b) {
    Buf800A991C buf;
    u8 result = 0;
    if (a != 0x14) {
        func_800ABB50(&buf, a, b);
        result = buf.value;
    }
    return result;
}
