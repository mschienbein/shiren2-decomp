#include "common.h"

/*
 * Static destructor of the 0x40-byte random-number generator at D_80147620
 * (the shape g++ 2.8.1 emits for a global object's destruction). The inline
 * base destructor reinstalls the base vtable D_80149DC8 at +0xC; the explicit
 * destructor call is expanded as a call before being inlined, which leaves the
 * 16-byte outgoing-argument area in the frame.
 */

extern "C" const s32 D_80149DC8[];

struct Rng800C5EE8 {
    s32 fields_00[3];
    const void *vtable;
    unsigned char state_10[0x30];
    ~Rng800C5EE8() { vtable = D_80149DC8; }
};

extern "C" Rng800C5EE8 D_80147620;

extern "C" void func_800C5EE8(void)
{
    D_80147620.~Rng800C5EE8();
}
