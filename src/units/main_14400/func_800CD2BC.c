#include "common.h"

typedef unsigned char u8;
typedef struct {
    u8 field_00[0x48];
    short field_48;
    short field_4A;
    void (*field_4C)(void *, s32);
} Methods;
typedef struct {
    s32 field_00;
    Methods *field_04;
} Object;
extern s32 func_800CD090(void *container, void *element);

s32 func_800CD2BC(Object *object, void *element) {
    s32 value = func_800CD090(object, element);
    if (value < 0) {
        return 0;
    }
    object->field_04->field_4C((u8 *)object + object->field_04->field_48, value);
    return 1;
}
