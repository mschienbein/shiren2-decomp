#include "common.h"
extern void func_80130F90(void *file, s32 offset, s32 size, void *buffer, s32 flag);
void func_80058DE4(void *file, s32 offset, s32 size, void *buffer, s32 flag) { func_80130F90(file, offset, size, buffer, flag); }
