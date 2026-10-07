#include "common.h"
typedef unsigned char u8;
extern s32 func_8010B920(void *obj, u8 id, void *table);
extern u8 D_801484C0[];
/* Unit-family slot +0x44 stat(self, id) (D_8015D518+0x44): the byte result is unsigned char,
 * as every unit call site narrows it (andi 0xFF) and both overrides produce it narrowed. */
u8 func_8011148C(void *obj, s32 id) { return func_8010B920(obj, id, D_801484C0); }
