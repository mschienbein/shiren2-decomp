#include "common.h"

/* Partial view: only the word read here is known. */
typedef struct {
    char pad0[0x58];
    u32 field_58;
} Obj;

u32 func_8009AD24(Obj *obj)
{
    return obj->field_58;
}
