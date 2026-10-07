#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 flag; u8 pad; u16 value; } Rec4;
typedef struct { u8 flag; u8 value; } Rec2;
/* Partial views of the two state blocks at 0x80142F1B and 0x80142F24. */
typedef struct {
    u8 flags;       /* 0x80142F1B */
    u8 pad1C[4];
    u8 mode;        /* 0x80142F20 */
    u8 a;           /* 0x80142F21 */
    u8 b;           /* 0x80142F22 */
} StateA;
typedef struct {
    u8 sel0;        /* 0x80142F24 */
    u8 sel1;        /* 0x80142F25 */
    u8 pad26[5];
    u8 sel1Prev;    /* 0x80142F2B */
    u8 pad2C;
    signed char result; /* 0x80142F2D */
} StateB;
extern StateA D_80142F1B;
extern StateB D_80142F24;
extern Rec4 D_80142E88[];
extern Rec2 D_80142D28[];
extern u16 D_80142B14;
extern u16 D_80142B16;
void func_800ABBA0(u8 a, u8 b);
s32 func_800A99A8(void);
void func_800A9C8C(u8 a, u8 b) {
    s32 i;
    s32 other;
    D_80142F24.sel0 = a;
    D_80142F24.sel1 = b;
    func_800ABBA0(a, b);
    D_80142B14 = 0;
    for (i = 0; D_80142E88[i].flag != 0; i++) D_80142B14 += D_80142E88[i].value;
    D_80142B16 = 0;
    for (i = 0; D_80142D28[i].flag != 0; i++) D_80142B16 += D_80142D28[i].value;
    if (func_800A99A8() && D_80142F24.sel1 == D_80142F24.sel1Prev) {
        D_80142F1B.a = 7;
        D_80142F1B.b = 7;
        D_80142F1B.flags |= 0x50;
    }
    other = (D_80142F1B.mode & 0xE0) != 0x40;
    if (other) D_80142F24.result = -1;
}
