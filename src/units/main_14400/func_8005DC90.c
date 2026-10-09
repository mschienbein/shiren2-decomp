#include "common.h"

/* func_8005DC90 and func_8005DCA4 both clear one initialized word; the empty
 * func_8005DC9C lies between them. One definition owned by this unit. */
s32 D_8013B600 = 0;

void func_8005DC90(void) {
    D_8013B600 = 0;
}

void func_8005DC9C(void) {
}

void func_8005DCA4(void) {
    D_8013B600 = 0;
}
