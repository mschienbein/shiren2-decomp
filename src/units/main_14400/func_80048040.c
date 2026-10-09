#include "common.h"
/* Complete 0x5C-byte menu (.bss 0x80160BC0..0x80160C1B): func_80097240 stores +0x50, +0x54
 * and +0x58; the static destructor func_80048024 stores the +0x4C vtable slot. */
typedef struct { unsigned char pad0[0x4C]; const void *field_4C; unsigned char pad_50[0xC]; } Obj;
typedef Obj Menu80096140;
typedef struct MenuItem80096140 MenuItem80096140;
typedef struct Layout80096140 Layout80096140;
typedef struct Selection80096140 Selection80096140;
extern Obj D_80160BC0;
extern const unsigned char D_801521D0[144];
extern MenuItem80096140 D_80138C70;
extern Layout80096140 D_80138C94;
extern Selection80096140 D_80138CA4;
extern Obj *func_800953C0(Obj *o);
extern void func_80097240(Menu80096140 *menu, MenuItem80096140 *items, Layout80096140 *layout, Selection80096140 *sel);
void func_80048040(void) {
    func_800953C0(&D_80160BC0);
    D_80160BC0.field_4C = D_801521D0;
    func_80097240(&D_80160BC0, &D_80138C70, &D_80138C94, &D_80138CA4);
}
