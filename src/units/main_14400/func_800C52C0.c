#include "common.h"

typedef unsigned short u16;
typedef unsigned char u8;

typedef struct {
    s32 first;
    s32 second;
} Pair;

typedef struct {
    Pair pair;
    u16 field_08;
    u16 field_0A;
    u16 field_0C;
    u16 field_0E;
    u8 field_10;
    u16 field_12;
} Obj800C52C0;

void func_800C52C0(Obj800C52C0 *p, Pair *v, unsigned short c)
{
    p->pair = *v;
    p->field_08 = 0;
    p->field_0A = c;
    p->field_0C = 0;
    p->field_0E = 0;
    p->field_10 = 0;
    p->field_12 = 0;
}
