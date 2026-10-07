#include "common.h"

extern char D_8014AA90[];
void func_800547E0(s32 arg0, const char *fmt, void *args);

void func_80048334(s32 arg0, const char *fmt, ...) {
    func_800547E0(arg0, D_8014AA90, __builtin_next_arg(fmt));
}
