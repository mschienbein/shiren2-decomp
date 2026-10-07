#include "common.h"

typedef struct {
    u32 field_00;
    unsigned char field_04;
} Object;
extern s32 func_8008C400(void);
extern void func_8008C75C(Object *);

s32 func_8008C89C(Object *object) {
    s32 result = 0;
    if ((object->field_00 & 0xFFFF0000) == 0x01000000) {
        if (object->field_04 == 0 || func_8008C400()) {
            func_8008C75C(object);
            result = 1;
        }
    }
    return result;
}
