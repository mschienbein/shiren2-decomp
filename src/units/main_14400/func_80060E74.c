#include "common.h"
typedef unsigned char u8;
typedef struct { u32 command, argument; } Gfx;
typedef struct Pool Pool;
extern Pool D_801E4E48;
extern u8 D_8013B74C[];
extern s32 func_8008BE24(u32 size);
extern void func_800612D4(s32 value);
extern void func_8005CB90(void);
extern void func_8006E7E0(void);
extern void func_800553B0(void);
extern s32 func_800718CC(u32 mode, void *a, void *b);
extern void func_80056D60(void);
extern s32 func_8007D480(void);
extern void func_80054DE0(s32 value);
/* The setter stores a callback, and 80064444 consumes and returns a Gfx cursor. */
extern void func_80055068(Gfx *(*callback)(Gfx *));
extern Gfx *func_80064444(Gfx *cursor);
extern s32 func_8006E908(void *pool, s32 a, s32 b, s32 c);
extern u8 func_8006C508(u8 value);
void func_80060E74(void) {
    func_8008BE24(0x19000);
    func_800612D4(1);
    func_8005CB90();
    func_8006E7E0();
    func_800553B0();
    func_800718CC(0, 0, D_8013B74C);
    func_80056D60();
    func_8007D480();
    func_80054DE0(2);
    func_80055068(func_80064444);
    func_8006E908(&D_801E4E48, 0x1900, 0x1100, 0x180);
    func_8006C508(2);
}
