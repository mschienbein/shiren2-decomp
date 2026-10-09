#include "common.h"

/* Three per-index values precede the embedded subobject whose vtable pointer
 * the constructor (func_8009A514) stores at 0x5C. */
typedef struct {
    char pad0[0x50];
    s32 values[3];
} Obj8009ACC8;

/* Vtable slot shared with func_80097630: (object, index) -> s32. */
s32 func_8009ACC8(Obj8009ACC8 *obj, s32 index) {
    return obj->values[index];
}
