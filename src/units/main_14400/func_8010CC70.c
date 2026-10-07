#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 field_00;
    u8 field_01;
    u8 field_02[6];
    void *field_08;
    u8 field_0C;
    u8 field_0D;
} Object;
extern s32 D_8015D1E0[];
extern u8 D_8015699F;
extern void func_8010B8D0(Object *, s32, s32);
extern void func_8010CCD0(Object *);

Object *func_8010CC70(Object *object, s32 value) {
    func_8010B8D0(object, 4, value);
    object->field_08 = D_8015D1E0;
    func_8010CCD0(object);
    if (object->field_01 == 0x64) {
        object->field_0D = D_8015699F;
    }
    return object;
}
