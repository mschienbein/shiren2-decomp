#include "common.h"

extern s32 func_800ACEB4(void *item);
extern char *func_801116E0(void *object, char *buffer, s32 type);

char *func_8011186C(void *object, char *buffer)
{
    return func_801116E0(object, buffer, func_800ACEB4(object));
}
