#include "common.h"

void func_800ACD34(void *obj);

/* Container slot +0x44 supplies self and actor; this override ignores both. */
void *func_80120890(void *a0, void *a1, void *a2)
{
    func_800ACD34(a2);
    return a2;
}
