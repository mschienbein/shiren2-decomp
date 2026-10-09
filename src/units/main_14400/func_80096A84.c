#include "common.h"
typedef struct { unsigned char pad_0[0x4C]; const void *vtable_4C; unsigned char pad_50[0xC]; } Obj;
typedef Obj Menu80096140;
typedef unsigned short u16;
/* Same 12-byte record definition as func_80096140 (D_80140260). */
typedef struct {
    u16 id;
    char pad2[0x8 - 0x2];
    s32 value;
} MenuItem80096140;
typedef struct { s32 columns, width, x, y; } Layout80096140;
typedef struct { s32 count, width; } Selection80096140;
extern Obj D_80140278, D_80140304, D_80140390;
extern MenuItem80096140 D_80140260[2], D_801402D4[4], D_80140360[4];
extern Layout80096140 D_80138D98, D_80138DB0, D_80138DC8;
extern Selection80096140 D_80138DA8, D_80138DC0, D_80138DD8;
extern const unsigned char D_801521D0[144];
extern Obj *func_800953C0(Obj *o);
extern void func_80097240(Menu80096140 *menu, MenuItem80096140 *items, Layout80096140 *layout, Selection80096140 *sel);
void func_80096A84(void) {
    func_800953C0(&D_80140278);
    D_80140278.vtable_4C=D_801521D0;
    func_80097240(&D_80140278,D_80140260,&D_80138D98,&D_80138DA8);
    func_800953C0(&D_80140304);
    D_80140304.vtable_4C=D_801521D0;
    func_80097240(&D_80140304,D_801402D4,&D_80138DB0,&D_80138DC0);
    func_800953C0(&D_80140390);
    D_80140390.vtable_4C=D_801521D0;
    func_80097240(&D_80140390,D_80140360,&D_80138DC8,&D_80138DD8);
}
