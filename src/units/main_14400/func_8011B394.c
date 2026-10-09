#include "common.h"

typedef unsigned short u16;
extern s32 func_80049CB4(s32 id, ...);
extern void func_80049A04(u16 id, ...);

/* Scroll slot +0x44 supplies self, actor and target; this effect uses none of them. */
void func_8011B394(void *self, void *actor, void *target) {
    func_80049CB4(0x132);
    func_80049A04(0xDE);
}
