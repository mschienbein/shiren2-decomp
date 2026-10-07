#include "common.h"

/* Partial view: only the word read here is known. */
typedef struct {
    char pad0[0x58];
    s32 field_58;
} Obj;

/* Widget vtable slot 13 (+0x68 this-adjust, +0x6C method): value of a flattened item
 * index. This single-value class returns its one value for any index; index is passed
 * by the slot contract and unused. */
s32 func_80097A28(Obj *obj, s32 index)
{
    return obj->field_58;
}
