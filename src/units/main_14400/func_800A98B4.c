#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 field_0; u8 field_1; u8 pad2[0xE]; } Buf800A98B4;
void func_800ABB50(Buf800A98B4 *out, u8 a, u8 b);
char *func_80048480(u16 id);

void func_800A98B4(u8 a, u8 b) {
    Buf800A98B4 buf;
    u16 id;
    switch (a) {
    case 9:
    case 10:
    case 20:
        id = 0x3E81;
        break;
    default:
        func_800ABB50(&buf, a, b);
        id = buf.field_1 + 0x3E81;
        break;
    }
    func_80048480(id);
}
