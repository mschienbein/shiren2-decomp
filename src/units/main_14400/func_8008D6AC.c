#include "common.h"
typedef struct { s32 field_0; s32 field_4; void *field_8; s32 field_C; void *field_10; } Record;
typedef struct { unsigned char field_0; unsigned char pad_1[3]; Record *field_4; } Obj;
extern void func_80091544(void *item);
void func_8008D6AC(Obj *obj) {
    s32 i;
    if (obj->field_4) {
        for (i = 0; i < obj->field_0; i++) {
            if (obj->field_4[i].field_10) func_80091544(obj->field_4[i].field_10);
            if (obj->field_4[i].field_8) func_80091544(obj->field_4[i].field_8);
        }
        func_80091544(obj->field_4);
        obj->field_4 = 0;
    }
    obj->field_0 = 0;
}
