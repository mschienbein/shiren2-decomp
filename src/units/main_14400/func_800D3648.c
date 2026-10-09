#include "common.h"

/* Region record (D_80143330): owner byte +0 plus 3 padding bytes, word +4. */
typedef struct {
    char pad0[4];
    s32 field_4;
} Obj800D3648;

void func_800D3648(Obj800D3648 *obj, s32 value) {
    obj->field_4 = value;
}
