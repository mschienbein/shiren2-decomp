#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { u32 pad : 10; u32 boost : 1; u32 rest : 21; } Bits;
typedef struct { u8 pad[0x20]; Bits bits; } Obj;
typedef struct { void *x0; s32 x4; s32 x8; s16 power; u16 flags; u8 x10; u8 pad11[3]; } Attack;
typedef struct { s32 x, y; } Vec2;
extern s32 func_800E8DC8(Obj *, s16);
extern void *func_800A6CC0(Vec2 *, Obj *);
extern s32 func_800B4888(Vec2 *);
extern s32 func_800C587C(void *, u8);
extern s32 func_800E33D0(Obj *, Attack, u8, u8);
extern u8 D_80147620[];
extern u8 D_80156985;
extern u8 D_80156987;
extern u8 D_80156A13;
static inline void copyBits(Bits *out, const Bits *in) {
    *out = *in;
}
void func_800E8FAC(Obj *o){
    Attack atk;
    Attack *pa = &atk;
    Vec2 pos;
    Attack arg;
    pa->power = func_800E8DC8(o, 0);
    pa->flags = 0;
    pa->x8 = 0;
    pa->x10 = 10;
    func_800A6CC0(&pos, o);
    if (func_800B4888(&pos)) {
        if (func_800C587C(D_80147620, D_80156985)) {
            s32 p = pa->power * D_80156987 / 10;
            pa->power = (p > 0x7FFF) ? 0x7FFF : p;
            atk.flags |= 0x80;
        } else {
            Bits b;
            copyBits(&b, &o->bits);
            {
                u32 boost = b.boost;
                if (boost) {
                    s32 p = pa->power * D_80156A13 / 10;
                    pa->power = (p > 0x7FFF) ? 0x7FFF : p;
                    atk.flags |= 0x100;
                }
            }
        }
    }
    arg = atk;
    func_800E33D0(o, arg, 1, 1);
}
