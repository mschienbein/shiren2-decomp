#include "common.h"
typedef struct { s32 x, y; } Pair;
typedef struct { Pair first, second; } Rectangle;
extern s32 func_800A251C(Pair *a, Pair *b);
s32 func_800A2FFC(void *a, void *b) {
    Rectangle *left = a, *right = b;
    return func_800A251C(&left->first, &right->first) && func_800A251C(&left->second, &right->second);
}
