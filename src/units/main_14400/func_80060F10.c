#include "common.h"

typedef unsigned char u8;
typedef struct Gfx { u32 words[2]; } Gfx;
typedef struct RenderPool RenderPool;
extern s32 func_8008BE24(u32 size);
extern void func_800612D4(s32 value);
extern void func_8006E7E0(void);
extern s32 func_800718CC(u32 index, void *data, void *name);
extern void func_80054DE0(s32 mode);
extern Gfx *func_80064444(Gfx *display_list);
/* D_801630B8 is invoked at 0x800551CC and its advanced Gfx pointer consumed. */
extern void func_80055068(Gfx *(*callback)(Gfx *));
extern s32 func_8006E908(void *pool, s32 count1, s32 count2, s32 count3);
extern u8 func_8006C508(u8 value);
extern const char D_8013B74C[];
extern RenderPool D_801E4E48;

void func_80060F10(void)
{
    func_8008BE24(0x32000);
    func_800612D4(0);
    func_8006E7E0();
    func_800718CC(0, 0, (void *)D_8013B74C);
    func_80054DE0(2);
    func_80055068(func_80064444);
    func_8006E908(&D_801E4E48, 0x1900, 0x1100, 0x180);
    func_8006C508(2);
}
