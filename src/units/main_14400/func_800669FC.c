#include "common.h"
typedef struct { char pad[0x10]; unsigned char *f10; } T;
extern T *D_8013B980;
s32 func_800669FC(s32 i) {
    T *t = D_8013B980;
    s32 r = 0;
    if (t != 0) {
        if (i >= 0) {
            r = (t->f10 + i)[6];
        }
    }
    return r;
}
