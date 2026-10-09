#include "common.h"

typedef unsigned char u8;
typedef struct Object Object;
extern void *func_800A38FC(s32 size);
extern Object *func_80100050(Object *object, u8 value);

Object *func_80100010(u8 value, Object *object) {
    if (object != 0) {
        return func_80100050(object, value);
    } else {
        return func_80100050(func_800A38FC(0xA0), value);
    }
}
