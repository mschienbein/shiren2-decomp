#include "common.h"
typedef struct { unsigned char pad[0x4C]; unsigned char field4C; } Object;
unsigned char *func_800C480C(unsigned char *out, Object *p) { *out = p->field4C; return out; }
