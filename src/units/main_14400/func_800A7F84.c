#include "common.h"
extern char *func_800A7DE4(void *obj);
extern void *func_800A7DEC(void *obj);
extern s32 func_800A4CC4(void *obj, void *value, void *arg);
s32 func_800A7F84(void *obj) {
    char *arg = func_800A7DE4(obj);
    return func_800A4CC4(obj, func_800A7DEC(obj), arg);
}
