#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 a; u8 b; u8 c; } Triple800AA2BC;
extern Triple800AA2BC D_80142F20;

void func_800AA2BC(u8 a, u8 b, u8 c) {
    D_80142F20.a = a;
    D_80142F20.b = b;
    D_80142F20.c = c;
}
