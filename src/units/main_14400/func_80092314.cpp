#include "common.h"

/* g++ 2.8.1 unit: the original is a pointer-to-member-function call. The
 * g++ 2.x member pointer is {short delta; short index; pfn / short delta2}:
 * index > 0 selects a virtual slot through the vtable found at target+delta2,
 * whose 8-byte entries are {short delta; short index; pfn}. The polymorphic
 * target class only declares its virtual method, so no vtable is emitted here. */
struct Target {
    virtual void run(void);
};

typedef void (Target::*Method)(void);

/* Bound call record: target object at +0x4, member pointer at +0x8. */
struct BoundCall {
    s32 unk_00;
    Target *target;
    Method method;
};

extern "C" void func_80092314(BoundCall *call)
{
    (call->target->*call->method)();
}
