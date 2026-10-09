#include "common.h"

/* One initialized flag shared by the adjacent setter and getter. */
s32 D_8013D440 = 0;

void func_8006E74C(void) {
    D_8013D440 = 0;
}

s32 func_8006E758(void) {
    return D_8013D440;
}
