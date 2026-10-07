#include "common.h"

typedef struct { char pad[0x80]; s32 field80; } Object;
extern s32 func_8009BCF8(Object *, void *), func_8009BB74(Object *, void *);
s32 func_8009BB38(Object *obj, void *sel) {
    s32 result;
    if (obj->field80 != 3) {
        result = func_8009BCF8(obj, sel);
    } else {
        result = func_8009BB74(obj, sel);
    }
    return result;
}
