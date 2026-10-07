#include "common.h"
extern s32 func_800A3050(void *);
extern s32 func_800A31C8(void *rect, void *pos);
s32 func_800BAAE0(void *object, void *position){ void *member = (unsigned char *)object + 0x9AC; if (func_800A3050(member) != 0) return 0; return func_800A31C8(member, position); }
