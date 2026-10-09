#include "common.h"
typedef unsigned char u8;
typedef struct { char pad0[0x80]; void *field80; } Unit;
typedef struct { char pad0[5]; signed char field5; } Obj;
/* Two 0x18-byte records (splat labels D_80143330 and D_80143348): owner byte +0,
 * area pointer +4 (func_800D2A64), remaining bytes not interpreted here. */
typedef struct { signed char owner; u8 pad1[3]; void *area; u8 pad8[0x10]; } Record;
/* Entry iterator built by func_800B07F0: collection pointer +0, halfword cursor +4. */
typedef struct { void *collection; unsigned short cursor; } Iterator;
extern Record D_80143330[2];
extern u8 D_80143448;
extern s32 func_800A9070(s32 *, s32);
extern Unit *func_800A910C(s32 *);
extern void func_800F61D8(Unit *);
extern void func_800D37E8(s32);
extern Iterator *func_800B07F0(Iterator *);
extern s32 func_800B0808(Iterator *);
extern Obj *func_800B0864(Iterator *);
extern void func_800AE974(Obj *, signed char);
extern s32 func_800D2FB0(void *);
extern void func_800D3698(s32, s32);
void func_800D3C68(void) {
    Iterator position; s32 iterator = 0;
    for (;;) { Unit *p; if (!func_800A9070(&iterator, 87)) break; p = func_800A910C(&iterator); p->field80 = D_80143330; func_800F61D8(p); }
    func_800D37E8(1);
    if (D_80143448 == 2) { func_800B07F0(&position); for (;;) { Obj *p; if (!func_800B0808(&position)) break; p = func_800B0864(&position); if (p->field5 == 1) func_800AE974(p, 0); } func_800D3698(0, func_800D2FB0(&D_80143330[1])); }
}
