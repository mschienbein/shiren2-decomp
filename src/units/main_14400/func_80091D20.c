#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;

/*
 * Inline label subobject at window +0x18 (0x4C bytes): constructed by func_80092160
 * (16-byte text objects at +0x00/+0x10, text pointer +0x20, five name bytes +0x24,
 * 16-byte objects at +0x2C/+0x3C) and released by func_80092284.
 */
typedef struct {
    s32 text0[4];
    s32 text1[4];
    char *text;
    u8 name[5];
    u8 pad29[3];
    s32 obj2C[4];
    s32 obj3C[4];
} WindowLabel;

/* Partial window view: this function only forwards the label subobject. */
typedef struct { u8 pad[0x18]; WindowLabel label; } Window;
extern u16 D_8015470C[];
extern u8 D_801513A0;
extern u16 D_801513A4[];
extern char D_801C3430[];
void *func_800C9E00(void);
s32 func_800D1488(void *, u8);
s32 func_800D1A70(void *, s32, void *);
char *func_80048480(u16);
char *func_80083C90(char *, char *);
s32 func_800D1928(void *, s32);
s32 func_800D194C(s32);
s32 func_800D19C0(void *, s32);
u8 func_801E99C4(u8, s32);
s32 func_8005EF30(char *, const char *, ...);
char *func_80083D04(char *, char *);
void func_80092160(WindowLabel *, char *, u8 *);
void func_80092284(WindowLabel *);
void func_80091D20(Window *win, u32 slot) {
    void *ctx = func_800C9E00();
    s32 rank;
    s32 state;
    s32 i;
    s32 first;
    s8 marks[8];
    char name[8];
    char extra[16];

    if (slot < 5) {
        rank = func_800D1488(ctx, (u8)slot);
        func_800D1A70(ctx, (s32)slot, marks);
        func_80083C90(name, func_80048480(D_8015470C[slot]));
        switch (func_800D194C(func_800D1928(ctx, (s32)slot))) {
        case 3:
            func_80083C90(extra, func_80048480(0x208));
            break;
        case 1:
            func_80083C90(extra, func_80048480(0x206));
            break;
        case 2:
            func_80083C90(extra, func_80048480(0x207));
            break;
        default:
            extra[0] = D_801513A0;
            break;
        }
        state = func_800D19C0(ctx, (s32)slot);
        if (state == 4) {
            switch (rank) {
            case -2:
            case -1:
                if (func_801E99C4(slot, 1) == state) {
                    func_8005EF30(D_801C3430, func_80048480(0x51C), name);
                } else {
                    func_8005EF30(D_801C3430, func_80048480(0x51B), name);
                }
                break;
            case -3:
                func_8005EF30(D_801C3430, func_80048480(0x51E), name);
                break;
            default:
                func_8005EF30(D_801C3430, func_80048480(0x51D), name, rank + 1, extra);
                break;
            }
        } else {
            if (rank >= 0) {
                func_8005EF30(D_801C3430, func_80048480(0x518), extra, name);
            } else {
                first = 0;
                func_8005EF30(D_801C3430, func_80048480(0x519), name);
                for (i = 0; i < 5; i++) {
                    if (marks[i] != -2) continue;
                    if (first) func_80083D04(D_801C3430, func_80048480(0x517));
                    first = 1;
                    func_80083D04(D_801C3430, func_80048480(D_801513A4[i]));
                }
                func_80083D04(D_801C3430, func_80048480(0x51A));
            }
        }
        if (rank >= 0) {
            func_80092160(&win->label, D_801C3430, (u8 *)marks);
        } else {
            func_80092160(&win->label, D_801C3430, 0);
        }
    } else {
        func_80092284(&win->label);
    }
}
