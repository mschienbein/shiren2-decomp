#include "common.h"

extern s32 func_800F4A70(void *,s32 *),func_800A8F6C(s32 *);
extern void *func_800A910C(s32 *);
extern s32 func_800A533C(void *);
s32 func_800F51EC(void *obj,s32 *event) { if(*event==10) { s32 it; func_800F4A70(obj,event); it=0; while(func_800A8F6C(&it)) func_800A533C(func_800A910C(&it)); return 1; } else return func_800F4A70(obj,event); }
