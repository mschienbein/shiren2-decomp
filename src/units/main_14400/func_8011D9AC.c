#include "common.h"

extern s32 func_80112674(void *self, s32 kind);

s32 func_8011D9AC(void *self, s32 kind)
{
    if (func_80112674(self, kind)) {
        return 1;
    }
    return kind == 0x21;
}
