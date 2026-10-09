#include "common.h"

typedef unsigned short u16;
typedef struct { char pad0[0xC]; void *vtable; } S;
extern char D_8015B808[];
S *func_800C4BC0(S *s, void *owner, void *target, u16 c);
S *func_80102788(S *s, void *owner, void *target, u16 c) {
    func_800C4BC0(s, owner, target, c);
    s->vtable = D_8015B808;
    return s;
}
