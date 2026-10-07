#include "common.h"

/* The container prefix stores its pool/descriptor pointer at +0. */
typedef struct { void *pool; } Container;

void *func_800CE5DC(Container *container) {
    return container->pool;
}
