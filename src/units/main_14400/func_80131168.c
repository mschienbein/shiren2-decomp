#include "common.h"

typedef unsigned char u8;
typedef struct { u8 code; u8 key; } CharMap;
extern CharMap D_80148DC0[];
void func_80131168(u8 *src, u8 *dst, u32 len) {
    u32 i;
    for (i = 0; i < len; i++, src++) {
        u8 c = *src;
        if (c == 0) {
            *dst = c;
        } else if ((u8)(c - 0x1A) < 0x1A) {
            *dst = c + 0x27;
        } else if ((u8)(c - 0x10) < 10) {
            *dst = c + 0x20;
        } else if ((u8)(c - 0x50) < 0x2C) {
            *dst = c + 0x61;
        } else if ((u8)(c - 0x7C) < 0xF) {
            dst[0] = c + 0x3A;
            dst[1] = 0xDE;
            dst++;
        } else if ((u8)(c - 0x8B) < 5) {
            dst[0] = c + 0x3F;
            dst[1] = 0xDE;
        } else if ((u8)(c - 0x90) < 5) {
            dst[0] = c + 0x3A;
            dst[1] = 0xDF;
        } else {
            s32 j;
            for (j = 0; D_80148DC0[j].key != 0; j++) {
                if (D_80148DC0[j].key == c) {
                    *dst = D_80148DC0[j].code;
                    break;
                }
            }
            if (D_80148DC0[j].key == 0) {
                *dst = 0xF;
            }
        }
        dst++;
    }
}
