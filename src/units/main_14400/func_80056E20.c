#include "common.h"
typedef struct Sprite Sprite;
extern s32 D_8013A290, D_801630D0, D_801630D4;
extern Sprite *D_80163100;
extern char D_801630D8[];
extern void func_8006E8E0(void *);
void func_80056E20(void) { D_8013A290 = 0; D_801630D0 = 0; D_801630D4 = 0; func_8006E8E0(D_801630D8); D_80163100 = 0; }
