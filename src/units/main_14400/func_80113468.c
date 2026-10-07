#include "common.h"

/* The query slot supplies self; this override only inspects kind. */
s32 func_80113468(void *self, s32 kind)
{
    s32 result = 0;

    switch (kind) {
    case 5:
    case 11:
        result = 1;
        break;
    }
    return result;
}
