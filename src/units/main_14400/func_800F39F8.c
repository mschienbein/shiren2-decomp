#include "common.h"

typedef unsigned char u8;
typedef struct { char pad[0x9E]; u8 f9E; } S;
u8 func_800F39F8(S *s) { return s->f9E; }
