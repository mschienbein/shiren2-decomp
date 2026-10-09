#include "common.h"

typedef unsigned char u8;

typedef struct Member Member;

typedef struct {
    u8 pad00[4];
    u8 level04;
} Stat;

extern s32 func_800CD278(Member *);
extern char *func_800AC990(void *obj);
extern void func_800498E4(s32 id, ...);

s32 func_800CD01C(Member *self, Stat *have, Stat *need, s32 notify)
{
    s32 ok = func_800CD278(self) + have->level04 >= need->level04;

    if (notify && !ok) {
        func_800498E4(0x86, func_800AC990(need));
    }
    return ok;
}
