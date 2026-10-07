#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { s32 unk0; u8 unk4[0x10]; u16 unk14; char *unk18; } S;
void func_80048764(void *);
void func_80048870(void *, s32);
char *func_80048480(u16);
void func_800487EC(void *, s32, s32, char *);
void func_80095114(S *arg0) {
    void *p = arg0->unk4;
    func_80048764(p);
    func_80048870(p, 0x78000000);
    if (arg0->unk14 != 0) {
        func_800487EC(p, 0, 0, func_80048480(arg0->unk14));
    } else {
        func_800487EC(p, 0, 0, arg0->unk18);
    }
}
