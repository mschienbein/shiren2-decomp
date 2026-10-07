#include "common.h"

typedef struct { char pad[0x10]; void *unk10; } S;
extern char *func_800AC9D8(void *);
char *func_800C4C38(S *s) { return func_800AC9D8(s->unk10); }
