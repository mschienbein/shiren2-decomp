#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { u8 pad[4]; s16 state; u8 pad6[2]; u16 phase; u8 padA[0x12]; s32 timer; } S;
void func_8008865C(S *s) {
    switch (s->phase) {
    case 0:
        s->phase++;
    case 1:
        if (s->timer-- == 0) {
            s->state = 4;
        }
        break;
    }
}
