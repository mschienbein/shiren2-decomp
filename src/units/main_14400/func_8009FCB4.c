#include "common.h"
typedef struct { unsigned char pad0[0x20]; s32 field_20; unsigned char pad24[0x10]; s32 field_34; s32 field_38; } Obj8009FCB4;
s32 func_8009FD08(Obj8009FCB4 *obj, s32 pos);
s32 func_8009FBB8(Obj8009FCB4 *obj, s32 flag);
s32 func_8009FCB4(Obj8009FCB4 *obj) {
    s32 pos = obj->field_34 + obj->field_20 * obj->field_38;
    if (pos < 0) return 0;
    return func_8009FBB8(obj, func_8009FD08(obj, pos) ^ 1);
}
