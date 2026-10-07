#include "common.h"

/* The query slot supplies self; this override only inspects kind. */
s32 func_8011ACB0(void *self, s32 kind)
{
    s32 result = 0;

    switch (kind) {
    case 6:
    case 11:
        result = 1;
        break;
    }
    return result;
}
