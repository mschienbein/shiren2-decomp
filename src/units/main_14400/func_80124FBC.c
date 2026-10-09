#include "common.h"
typedef struct { s32 x; s32 y; } Pos;
extern char D_8016005C[], D_8016008C[];
extern void *func_800B4D80(Pos *);
extern void func_80136964(const char *, ...);
extern s32 func_8011587C(void *, Pos *);
void func_80124FBC(void *p, Pos *position) { void *found = func_800B4D80(position); if (found != p) { func_80136964(D_8016005C, D_8016008C, 35); } else { func_8011587C(found, position); } }
