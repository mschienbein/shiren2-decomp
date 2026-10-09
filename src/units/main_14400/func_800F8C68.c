#include "common.h"
extern s32 func_800E8694(void *obj);
extern s32 func_800E66EC(void *obj);
extern s32 func_800E8350(void *obj);
s32 func_800F8C68(void *obj)
{
    if (func_800E8694(obj)) return func_800E8350(obj);
    return func_800E66EC(obj);
}
