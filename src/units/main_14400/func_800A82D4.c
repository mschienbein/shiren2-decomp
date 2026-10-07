#include "common.h"

typedef unsigned short u16;
typedef struct { char pad[0x1C]; u16 f1C; } S;
void func_800A82D4(S *a){a->f1C &= ~2;}
