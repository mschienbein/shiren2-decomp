#include "common.h"
typedef struct Sprite Sprite;
extern s32 D_801630D4, D_8013A290, D_801630D0;
extern Sprite *D_80163100;
/* Opaque 0x28-byte pool descriptor, cleared through +0x24 by func_8006E8E0. */
extern unsigned char D_801630D8[0x28];
extern void func_8006EA18(void *);
void func_80056FB8(void) { D_801630D4 = 0; func_8006EA18(D_801630D8); D_80163100 = 0; D_8013A290 = 0; D_801630D0 = 0; }
