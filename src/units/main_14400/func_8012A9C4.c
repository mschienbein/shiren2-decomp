#include "common.h"

/* Same channel-record pointer as the setter and active-bank consumers. */
typedef struct { char pad[0x10]; void *x10; } S;
void *func_8012A9C4(S *s) { return s->x10; }
