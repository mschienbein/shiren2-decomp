#include "common.h"

/* g++ 2.8.1 unit: destroys the global cursor menu D_80140130. The cursor-menu
 * destructor only reinstalls the root method table D_80151350 at +0xC. g++ 2.8
 * expands the destructor's in-charge `__builtin_delete` call before folding the
 * constant flag away, so the call's outgoing-argument area is still allocated.
 * That leaves the 16-byte frame with no saved registers, which C cannot express
 * without an unused local. The destructor is inline, so no mangled symbol is
 * emitted. */
extern "C" char D_80151350[];

struct MenuCursor {
    s32 index;
    void *records;
    s32 unknown_08;
    void *vtable_0C;
    ~MenuCursor() { vtable_0C = D_80151350; }
};

extern "C" MenuCursor D_80140130;

extern "C" void func_80092590(void)
{
    D_80140130.~MenuCursor();
}
