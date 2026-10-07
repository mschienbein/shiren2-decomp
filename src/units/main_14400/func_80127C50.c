#include "common.h"
extern void *D_801476B8;
extern char *func_80048480(unsigned short id);
extern char *func_800A3B20(void *obj);
extern char *func_800AC990(void *obj);
extern s32 func_8005EF08(char *dst, const char *fmt, ...);
void *func_80127C50(void *arg,void *out) { char *a=func_80048480(0x22D); char *b=func_800A3B20(D_801476B8); char *c=func_800AC990(arg); func_8005EF08(out,a,b,c); return out; }
