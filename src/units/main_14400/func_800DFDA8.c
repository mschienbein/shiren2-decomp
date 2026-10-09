#include "common.h"

/* Whole 0x70-byte object, including the member constructed at +0x18. */
typedef struct SharedMenu {
    s32 index; void *records; s32 unknown_08; void *vtable_0C;
    s32 active_10; s32 flag_14;
    struct {
        s32 unknown_00[3]; s32 value_0C;
        s32 unknown_10[3]; s32 value_1C;
        s32 unknown_20[3]; const void *vtable_2C;
        s32 unknown_30[3]; const void *vtable_3C;
        s32 unknown_40[6];
    } member_18;
} SharedMenu;
typedef struct Unit Unit;
extern SharedMenu D_80140080;
extern void *func_800C9E00(void);
extern void func_800D10C4(char *obj);
extern void func_80047A90(void *);
extern s32 func_80047AF4(void *, s32);
extern void func_80047AC0(void *self);
extern Unit *func_800C5F60(void);
extern void func_800C92BC(void *obj);

/* Virtual action slot +0x14 supplies an unused receiver. */
s32 func_800DFDA8(void *unused_receiver)
{
    func_800D10C4(func_800C9E00());
    func_80047A90(&D_80140080);
    func_80047AF4(&D_80140080, 0);
    func_80047AC0(&D_80140080);
    func_800C92BC(func_800C5F60());
    return 1;
}
