#include "common.h"

typedef struct { char pad[0x1C]; s32 f1C; s32 f20; s32 f24; } T;
extern s32 D_8013E90C;
void func_8008AF00(void *task); void func_8008B678(void *task);
void *func_80085154(void (*handler)(void *), s32 value);
void *func_800851B0(s32 id){
    T *t;
    if (D_8013E90C) { t = func_80085154(func_8008AF00, id); t->f1C = 0; t->f24 = 0; }
    else t = func_80085154(func_8008B678, 0);
    return t;
}
