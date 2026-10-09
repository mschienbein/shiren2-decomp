#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* 0x9C-byte record filled by func_800CCBB8 and consumed by func_8009E5A0. */
typedef struct {
    u8 b0;
    u8 b1;
    u8 pad02[6];
    u8 b8;
    u8 pad09[0x9C - 9];
} Block800CCDC0;

/* Menu result record: word 0 is the chosen entry; menu handlers may fill more. */
typedef struct {
    s32 values[8];
} MenuResult800CCDC0;

typedef struct Menu800CCDC0 Menu800CCDC0;

/* Initialized .data byte at its original address; this file owns it. */
u8 D_801476D1 = 0;
extern Menu800CCDC0 D_801426B0;
extern Menu800CCDC0 D_801424D0;
/* Whole menu-system object; its signed display-mode byte is at +4. */
extern signed char D_80140160[];

extern void func_80045A84(void);
extern void func_80046C30(s32 id);
extern s32 func_80046C6C(s32 mode);
extern void func_8009EEF0(Menu800CCDC0 *menu, u8 arg, s32 sel);
extern s32 func_80095AD8(Menu800CCDC0 *menu, MenuResult800CCDC0 *result, s32 c);
extern s32 func_800CCBB8(u8 arg0, u8 arg1, void *arg2);
extern void func_801F212C(u16 id, u8 flag);
extern s32 func_801EF340(s32 flag, s32 arg1);
extern void func_800A9DC0(u8 a, u8 b, s32 c);
extern int func_800413E0(void);
extern void func_800B1268(void);
extern void func_800413FC(void);
extern void func_80041450(s32 value);
extern void func_8009E5A0(Menu800CCDC0 *obj, Block800CCDC0 *src, s32 flag);

void func_800CCDC0(u8 arg) {
    MenuResult800CCDC0 result;
    Block800CCDC0 block;
    s32 loaded = 0;
    s32 sel = 0;

    D_801476D1 = 1;
    func_80045A84();
    for (;;) {
        if (!loaded) {
            func_80046C30(0);
            func_80046C6C(5);
            func_8009EEF0(&D_801426B0, arg, sel);
            if (!func_80095AD8(&D_801426B0, &result, 1)) {
                break;
            }
            loaded = 1;
            sel = result.values[0];
        } else {
            func_800CCBB8(arg, sel, &block);
            func_801F212C(0x462, 0);
            func_801EF340(D_80140160[4] == 1, 1);
            func_80046C30(3);
            func_800A9DC0(block.b0, block.b1, block.b8);
            loaded = 0;
            func_800413E0();
            func_800B1268();
            func_800413FC();
            func_80041450(0);
            func_8009E5A0(&D_801424D0, &block, sel < 10);
            func_80095AD8(&D_801424D0, &result, 1);
        }
    }
    D_801476D1 = 0;
    func_80045A84();
}
