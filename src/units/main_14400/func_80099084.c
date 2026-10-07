#include "common.h"
typedef struct { char pad[0x20]; s32 x20; } S;
s32 func_80098E34(S *p, s32 v);
s32 func_80099028(S *p, s32 idx);
s32 func_80099084(S *p, s32 key) {
    s32 idx = func_80098E34(p, key * p->x20);
    if (idx < 0) return -1;
    if (func_80099028(p, idx) != key) {
        idx++;
        if (func_80099028(p, idx) != key) return -1;
    }
    return idx;
}
