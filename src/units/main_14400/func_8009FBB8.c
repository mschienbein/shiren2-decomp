#include "common.h"
typedef struct { unsigned char pad0[0x20]; s32 field_20; unsigned char pad24[0x10]; s32 field_34; s32 field_38; unsigned char pad3C[0xEC]; s32 field_128; unsigned char field_12C[20]; } Obj8009FCB4;
typedef Obj8009FCB4 SA;
extern s32 func_8009FD08(SA *p, s32 id);
s32 func_8009FBB8(Obj8009FCB4 *obj, s32 flag) {
    s32 id = obj->field_34 + obj->field_20 * obj->field_38;
    s32 i;
    if (id < 0) return 0;
    if (flag) {
        s32 exists = func_8009FD08(obj, id) == 1;
        s32 count;
        if (exists) return 0;
        count = obj->field_128;
        if (count < 20) {
            obj->field_12C[count] = id;
            obj->field_128 = count + 1;
            return 1;
        }
    } else {
        for (i = 0; i < obj->field_128; i++) {
            if (obj->field_12C[i] == id) {
                obj->field_128--;
                while (i < obj->field_128) {
                    obj->field_12C[i] = obj->field_12C[i + 1];
                    i++;
                }
                return 1;
            }
        }
    }
    return 0;
}
