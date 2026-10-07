#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef void (*Method)(void *);
typedef struct { s16 delta; s16 index; Method pfn; } __attribute__((aligned(8))) VtEntry;
typedef struct { s16 delta; s16 index; union { Method pfn; s16 delta2; } u; } MemberFn;
typedef struct { s32 x0; u8 *obj; MemberFn method; } Callback;
void func_80095E78(Callback *cb) {
    VtEntry entry;
    Method fn;
    s32 off;
    s32 index = cb->method.index;
    if (index > 0) {
        entry = (*(VtEntry **)(cb->obj + cb->method.u.delta2))[index - 1];
        fn = entry.pfn;
    } else {
        fn = cb->method.u.pfn;
    }
    {
        s32 delta = cb->method.delta;
        off = index > 0 ? entry.delta + delta : delta;
    }
    fn(cb->obj + off);
}
