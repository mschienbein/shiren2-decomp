#include "common.h"

typedef struct { unsigned char pad00[0xC]; unsigned char flags0C; } Obj;

s32 func_8011349C(Obj *obj)
{
    return (obj->flags0C >> 1) & 1;
}
