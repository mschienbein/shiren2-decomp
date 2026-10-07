#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

void *func_8011422C(void *obj);
s32 func_800CD61C(void *container, void *item, s32 any);
/* Container slot +0x44 returns an item pointer; consuming it returns null.
 * The supplied actor (arg1) is unused by this override. */
void *func_80120970(void *arg0, void *arg1, void *arg2) {
    func_800CD61C(func_8011422C(arg0), arg2, 0);
    return 0;
}
