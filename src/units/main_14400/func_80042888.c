#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 fields[4]; } Settings;
extern Settings D_8014A734;
extern unsigned char D_801609C0[], D_80138AE0[];
extern void *func_800A8CB0(s32);
extern char *func_80048480(u16);
extern s32 func_800E0F40(void *);
extern s32 func_800327C0(void *, void *, ...);
extern void func_80095084(void *, void *, Settings *);
void func_80042888(s32 unit) {
    void *obj = func_800A8CB0((u8)unit);
    char *text = func_80048480(0x2A);
    u8 value = func_800E0F40(obj);
    Settings settings;
    func_800327C0(D_801609C0, text, value);
    settings = D_8014A734;
    func_80095084(D_80138AE0, D_801609C0, &settings);
}
