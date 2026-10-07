#include "common.h"
typedef struct { char pad[0x4C]; void *f4C; } SA;
extern char D_80151E38[];
void func_800D8FA8(void *object);
void func_8009D8A4(SA *p, s32 flags) {
    p->f4C = D_80151E38;
    if (flags & 1) {
        func_800D8FA8(p);
    }
}
