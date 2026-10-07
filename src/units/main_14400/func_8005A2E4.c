#include "common.h"
typedef struct { float x, y, z; } Vec3;
typedef struct { s32 x0, x4, x8; Vec3 eye; Vec3 at; float fov; float roll; } View;
typedef struct { Vec3 eye; Vec3 at; float fov; float roll; } ViewSrc;
extern void func_80059590(s32);
extern void func_800265E0(void *, s32);
extern s32 func_8005B2DC(View *, View *);
extern void func_8005B3EC(View *);
extern void func_8005C4AC(void *, void *);
extern s32 D_80165394, D_80165400, D_80165408, D_801653F8, D_801653FC, D_80165404;
extern View D_801653A0, D_801653CC;
extern Vec3 D_8016530C, D_80165324;
extern float D_8016533C, D_80165340;
extern char D_8016534C[], D_80165358[];
void func_8005A2E4(ViewSrc *src, s32 mode, s32 arg2){ View *cur; View *dst;
  func_80059590(1);
  cur = &D_801653A0;
  D_80165394 = 0;
  D_80165400 = 0;
  D_80165408 = 0;
  func_800265E0(cur, sizeof(View));
  dst = &D_801653CC;
  func_800265E0(dst, sizeof(View));
  cur->eye = D_8016530C;
  cur->at = D_80165324;
  cur->fov = D_8016533C;
  cur->roll = D_80165340;
  dst->eye = src->eye;
  dst->at = src->at;
  dst->fov = src->fov;
  dst->roll = src->roll;
  D_801653F8 = mode;
  D_801653FC = 0;
  D_80165404 = arg2;
  if (mode == 0 || func_8005B2DC(cur, dst)) {
    func_8005B3EC(dst);
    D_801653F8 = 0;
    func_8005C4AC(D_8016534C, D_80165358);
  } }
