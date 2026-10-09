#include "common.h"

typedef unsigned char u8;
typedef struct Owner Owner;
/* Whole 0x40-byte RNG object at D_80147620 (constructed by func_800C5D70): the depth word at
 * +8 is the interior splat label D_80147628. */
typedef struct {
    void *state_00;
    void *saved_04;
    s32 depth_08;
    const void *vtable_0C;
    u32 state_10[3];
    u32 saved_1C[9];
} RngObject;
extern RngObject D_80147620;
extern Owner D_801C9BD8;
extern u8 D_80148644[4];
extern u8 D_80142920[16];
extern s32 D_80142930;
extern u8 D_80138C40[16];
extern void func_800C5DC4(void *arg0);
extern void func_800D1830(Owner *g);
extern void func_800AF83C(void);
extern void func_801EF6C4(void);
extern void func_800B0890(void);
extern void func_800D7710(void);
extern void func_801224A0(void);
extern void func_800A15F0(void *obj);
static inline void rng_clear_depth(RngObject *rng) { rng->depth_08 = 0; }
extern void func_80046418(void *obj);

void func_800C6754(void) {
    u8 *p;
    s32 count;
    rng_clear_depth(&D_80147620);
    func_800C5DC4(&D_80147620);
    func_800D1830(&D_801C9BD8);
    func_800AF83C();
    func_801EF6C4();
    p = D_80148644;
    count = 3;
    do {
        *p++ = 0;
    } while (count-- > 0);
    func_800B0890();
    func_800D7710();
    func_801224A0();
    func_800A15F0(D_80142920);
    D_80142930 = 0;
    func_80046418(D_80138C40);
}
