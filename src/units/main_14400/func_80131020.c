#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { u8 from; u8 to; } Map;
extern Map D_80148DC0[];
void func_80131020(u8 *src, u8 *dst, u32 n) {
    u32 i;
    s32 j;
    u8 c;
    for (i = 0; i < n; i++, src++) {
        c = *src;
        if (c == 0) return;
        if (c >= 'A' && c <= 'Z') {
            *dst = c - 0x27;
        } else if ((u8)(c - 'a') < 26) {
            *dst = c - 0x47;
        } else if ((u8)(c - '0') < 10) {
            *dst = c - 0x20;
        } else if ((u8)(c - 0xB1) < 0x2C) {
            if (src[1] == 0xDE) {
                *dst = c < 0xCA ? c - 0x3A : c - 0x3F;
                src++;
            } else if (src[1] == 0xDF) {
                *dst = c - 0x3A;
                src++;
            } else {
                *dst = c - 0x61;
            }
        } else {
            for (j = 0; D_80148DC0[j].to != 0; j++) {
                if (D_80148DC0[j].from == c) {
                    *dst = D_80148DC0[j].to;
                    break;
                }
            }
            if (D_80148DC0[j].to == 0) *dst = 0xF;
        }
        dst++;
    }
}
