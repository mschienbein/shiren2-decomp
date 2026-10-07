#include "common.h"
typedef struct { unsigned char pad0[0xC]; s32 field_C; } Obj800487C4;
void func_800835AC(u32 index);
void func_800487C4(Obj800487C4 *obj) {
    if (obj->field_C >= 0) func_800835AC(obj->field_C);
}
