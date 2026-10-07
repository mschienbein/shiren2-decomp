#include "common.h"
void func_80097BE0(void *obj, void **buf, s32 mode, void *desc, s32 flag);
void func_80097B90(void *obj, void *owner, void *holder, s32 mode, void *desc, s32 flag) { void *buf[3]; s32 i; buf[0] = owner; buf[1] = holder; for (i = 2; i < 3; i++) buf[i] = 0; func_80097BE0(obj, buf, mode, desc, flag); }
