#include "common.h"

typedef unsigned char u8;
typedef struct { char pad[0x89]; u8 f89; } S;
u8 func_801072F8(S *s) { return s->f89; }
