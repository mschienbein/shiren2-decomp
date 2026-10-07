#include "common.h"

typedef struct { s32 x, y; } Pair;
typedef struct { char pad[0x10]; s32 x10; s32 y14; } Src;
Pair *func_8011BB8C(Pair *out, Src *src) { out->x = src->x10; out->y = src->y14; return out; }
