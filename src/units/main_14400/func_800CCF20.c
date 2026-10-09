#include "common.h"
/* Menu object at D_801424D0 (opaque here). */
typedef struct Menu800CCF20 Menu800CCF20;
/* 0x9C-byte source record that func_8009E5A0 copies into the menu object at +0x54. */
typedef struct Block800CCF20 Block800CCF20;
extern Menu800CCF20 D_801424D0;
extern void func_80041450(s32 value);
extern void func_8009E5A0(Menu800CCF20 *menu, const Block800CCF20 *source, s32 flag);
extern s32 func_800957C0(Menu800CCF20 *menu, void *output, s32 modal, void *history, s32 event);
void func_800CCF20(const Block800CCF20 *record)
{
    s32 output[8];
    func_80041450(0);
    func_8009E5A0(&D_801424D0, record, 1);
    func_800957C0(&D_801424D0, output, 0, 0, 0);
}
