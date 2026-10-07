#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 a;
    s32 b;
} Pair;

typedef struct {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
} Quad;

typedef struct {
    Pair pair;
    s32 unk8;
    Quad quad;
} Source;

typedef struct {
    s32 field_0;
    Source *source;
    Pair pair;
    Quad quad;
} Dest;

Dest *func_800A915C(Dest *dest, Source *src) {
    dest->source = src;
    dest->pair = src->pair;
    dest->quad = src->quad;
    dest->field_0 = 0;
    return dest;
}
