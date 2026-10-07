#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef struct { s8 value; s8 next; u8 pad[2]; } Step;
typedef struct { s32 count; Step *steps; } Sequence;
typedef struct { u8 pad0[7]; u8 active; u8 pad8[0x39]; u8 value; u8 index; s8 timer; } Player;
void func_80074E70(Player *p, Sequence *seq, s32 elapsed) {
    if (p->active == 1) {
        if (p->timer > 0) {
            if (elapsed < p->timer) {
                p->timer -= elapsed;
            } else {
                p->timer = 0;
            }
        }
        if (p->timer == 0) {
            if (seq->steps[p->index].value == -1 || p->index >= seq->count) {
                if (seq->steps[p->index].next == -1) {
                    p->index = 0;
                } else {
                    p->index = seq->steps[p->index].next;
                }
            }
            p->value = seq->steps[p->index].value;
            p->timer = seq->steps[p->index].next;
            p->index++;
        }
    }
}
