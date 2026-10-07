#include "common.h"

extern void *D_80140070;
extern void func_80091794(void *arg0);

void func_8009176C(void) {
    if (D_80140070 != 0) {
        func_80091794(D_80140070);
    }
}
