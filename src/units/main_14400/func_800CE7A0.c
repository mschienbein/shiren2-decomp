#include "common.h"

typedef struct {
    void *field_0;
    unsigned char pad4[4];
    unsigned char *field_8;
    unsigned char padC[2];
    unsigned char count;
} List;

void *func_800AFD78(void *a, unsigned char b);

void *func_800CE7A0(List *list, u32 index)
{
    if (index < list->count) {
        return func_800AFD78(list->field_0, list->field_8[index]);
    }
    return 0;
}
