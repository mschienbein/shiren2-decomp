#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16; typedef float f32;
typedef struct { u8 pad[0x4C]; void *vt; u8 pad50[0x18]; s32 x68; void *x6C; u8 pad70[0xA0]; } Dlg;
extern u8 D_80141AD0[]; extern u8 D_80154234[]; extern u16 D_80154238[]; extern u8 D_80138D50[]; extern u8 D_80138D60[];
extern u8 D_80152AE8[]; extern u8 D_80151EC8[]; extern u8 D_80151E38[]; extern u8 D_80147680[]; extern u8 D_80140166;
extern void func_8009ADE8(void*, s32, u8*, s32); extern s32 func_800957C0(void*, volatile s32*, s32, void*, s32); extern u8 *func_8009C17C(void*);
extern s32 func_80094E6C(s32); extern u8 *func_80083F34(u8*, s32, u8*); extern char *func_80048480(u16);
extern s32 func_8005EF08(char*, const char*, ...); extern Dlg *func_800953C0(Dlg*); extern void func_8009D610(Dlg*, char*, void*, void*);
extern s32 func_800CA76C(void*, u8); extern void func_800CA9A8(void*, u8*, s32);
static inline void dlg_set_vt(Dlg *d, void *vt){ d->vt = vt; }
s32 func_800C9E30(s32 arg){
  volatile s32 val; u8 name[0x10]; char text[0x80]; Dlg dlg; u8 *str; s32 n;
  func_8009ADE8(D_80141AD0, 4, D_80154234, 0);
  func_800957C0(D_80141AD0, &val, 1, 0, 0);
  str = func_8009C17C(D_80141AD0);
  if (*str == 0) str = D_80154234;
  n = func_80094E6C(1);
  if (n >= 3) n = 1;
  {
    char *fmt; s32 failed;
    func_80083F34(str, 9, name)[0] = 0;
    fmt = func_80048480(0x28E);
    func_8005EF08(text, fmt, (char *)name, func_80048480(D_80154238[n]));
    func_800953C0(&dlg); dlg_set_vt(&dlg, D_80152AE8); dlg.x68 = -1; dlg.x6C = D_80151EC8;
    func_8009D610(&dlg, text, D_80138D50, D_80138D60);
    failed = func_800957C0(&dlg, &val, 1, 0, 0) != 1;
    if (failed) val = 0;
    if (val) {
      func_800CA76C(D_80147680, arg);
      func_800CA9A8(D_80147680, str, n);
      D_80140166 = 0;
      dlg_set_vt(&dlg, D_80151E38);
      return 1;
    }
    dlg_set_vt(&dlg, D_80151E38);
    return 0;
  } }
