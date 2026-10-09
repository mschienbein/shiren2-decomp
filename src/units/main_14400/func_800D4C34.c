#include "common.h"

/* Pool container (cf. func_800D4D8C): pool-record pointer +0, +4 not accessed, mode byte +8. */
typedef struct { void *pool; unsigned char pad04[4]; signed char field08; } Object;
void func_800D4C34(Object *object) { object->field08 = -1; object->pool = 0; }
