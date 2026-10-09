#include "common.h"
typedef struct { unsigned char pad[0x1C]; s32 *field1C; } Object;
extern void func_800CAEF4(Object *);
extern void func_800CA0A8(s32 *, s32);
void func_800CAFC0(Object *obj) {
    s32 saved = *obj->field1C;
    func_800CAEF4(obj);
    func_800CA0A8(obj->field1C, saved);
}
