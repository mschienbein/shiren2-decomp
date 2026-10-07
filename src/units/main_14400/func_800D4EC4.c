#include "common.h"

extern char D_801430B4[];
extern char D_801430C4[];
extern char D_80147FA0[];
extern char D_80147FAC[];
char *func_800D4EC4(char *p) {
    if (p == D_801430B4) {
        return D_80147FA0;
    }
    if (p == D_801430C4) {
        return D_80147FAC;
    }
    return 0;
}
