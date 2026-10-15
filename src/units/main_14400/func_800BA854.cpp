#include "common.h"
#include "map_rect.h"

/*
 * Floor generator entry (C++ TU). The per-kind layout pass is invoked through a
 * g++ 2.x pointer-to-member-function table (D_80153CAC: {s16 delta, s16 index,
 * pfn or s16 vptr offset}); the call below is exactly g++'s member-pointer dispatch.
 */

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;

/* Partial view of the floor generator object. */
struct Generator_800BA854 {
    u8 pad0[4];
    s32 special4;
    u8 pad8[0x3D4];
    s32 field3DC;
    u8 pad3E0[8];
    s32 field3E8;
    u8 pad3EC[0xC];
    s32 field3F8;
    u8 pad3FC[4];
    s8 kind400;
    u8 pad401[0x557];
    u16 flags958;
};

/* Per-kind layout pass, a g++ 2.x pointer-to-member-function table ({delta, index, pfn/delta2}). */
typedef void (Generator_800BA854::*LayoutPass_800BA854)();

extern "C" {
extern LayoutPass_800BA854 const D_80153CAC[14];
extern u8 D_80143392;
extern u8 D_8014344C;
extern u8 D_80147620[];
void func_800435A8(void *self, s32 b);
void func_800B17A4(void);
s32 func_800B43BC(void *position, s32 arg1, u8 arg2);
void func_800B7884(void *generator);
void func_800BAB9C(Generator_800BA854 *self);
void func_800BAC30(Generator_800BA854 *self);
u16 func_800C58B0(void *rng);
}

extern "C" void func_800BA854(Generator_800BA854 *generator)
{
    u16 value;

    if (generator->special4) {
        func_800B7884(generator);
        MapRect area;
        {
            /* Set the corners directly. A brace initializer of MapPos
               temporaries zero-fills the rectangle first with a memset call
               (600 B and an undefined memset); the original stores the four
               words directly at 0x800BA89C..0x800BA8AC. */
            MapRect bounds;
            bounds.min.set(15, 17);
            bounds.max.set(19, 23);
            area.min = bounds.min;
            area.max = bounds.max;
        }
        func_800B7948(generator, 0, &area);
        MapPos *pos = &area.min;
        pos->set(16, 16);
        func_800B43BC(pos, 0, 0);
        pos->set(16, 15);
        func_800B43BC(pos, 0, 0);
        pos->set(17, 15);
        func_800B43BC(pos, 0, 0);
        pos->set(18, 15);
        func_800B43BC(pos, 0, 0);
        pos->set(18, 24);
        func_800B43BC(pos, 0, 0);
        pos->set(18, 25);
        func_800B43BC(pos, 0, 0);
        generator->field3DC = 1;
        D_8014344C = 1;
        func_800B17A4();
        D_80143392 = 0;
        value = func_800C58B0(D_80147620);
    } else {
        func_800B7884(generator);
        func_800BAC30(generator);
        generator->field3E8 = generator->flags958 & 1;
        generator->field3F8 = (generator->flags958 >> 9) & 1;
        (generator->*D_80153CAC[generator->kind400])();
        func_800BAB9C(generator);
        value = func_800C58B0(D_80147620);
    }
    func_800435A8(generator, value);
}
