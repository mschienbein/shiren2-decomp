#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef struct { u8 pad[0xC]; s32 end; u8 pad10[4]; s32 pos; u8 pad18[0x20]; u8 *data; u8 pad3C[0x66]; u16 wait; u8 padA4[0x18]; u8 value; } Track;
void func_8012BA20(Track *t) {
    u8 c;
    do {
        t->pos += 0x100;
        if (--t->wait == 0) {
            c = *t->data++;
            if ((s8)c < 0) {
                t->value = c & 0x7F;
                c = *t->data++;
                if ((s8)c < 0) {
                    t->wait = (c & 0x7F) << 8;
                    t->wait += *t->data++ + 2;
                } else {
                    t->wait = c + 2;
                }
            } else {
                t->value = c;
                t->wait = 1;
            }
        }
    } while (t->pos - t->end < 0);
}
