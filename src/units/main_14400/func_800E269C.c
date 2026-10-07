#include "common.h"

typedef unsigned char u8;
typedef struct { char pad[0x72]; u8 f72; } S;
void func_800E269C(S *s, u8 v) { s->f72 = v; }
