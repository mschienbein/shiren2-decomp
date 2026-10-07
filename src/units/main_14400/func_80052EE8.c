#include "common.h"

typedef struct { signed char a, b; } Pair8;
typedef struct { s32 x0; s32 x4; Pair8 pair8; } Node;
Node *func_80053034(short kind);
Node *func_80052EE8(short kind, s32 value, Pair8 pair) {
    Node *n = func_80053034(kind);
    n->x4 = value;
    n->pair8 = pair;
    return n;
}
