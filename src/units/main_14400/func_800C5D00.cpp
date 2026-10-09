#include "common.h"

/*
 * Static destructor of the 0x20-byte random-number generator at D_80147600
 * (the shape g++ 2.8.1 emits for a global object's destruction). The inline
 * base destructor reinstalls the base vtable D_80149DC8 at +0xC; the explicit
 * destructor call is expanded as a call before being inlined, which leaves the
 * 16-byte outgoing-argument area in the frame.
 */

extern "C" const s32 D_80149DC8[];

struct Rng800C5D00 {
    s32 fields_00[3];
    const void *vtable;
    unsigned char state_10[0x10];
    ~Rng800C5D00() { vtable = D_80149DC8; }
};

extern "C" Rng800C5D00 D_80147600;

extern "C" void func_800C5D00(void)
{
    D_80147600.~Rng800C5D00();
}
