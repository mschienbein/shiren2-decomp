#include "common.h"
typedef struct {
    short x0, y0;
    short x1, y1;
    short x2, y2;
    unsigned char unkC;
    unsigned char padD;
    unsigned char unkE, unkF, unk10, unk11, unk12, unk13;
    unsigned char flags14;
    unsigned char pad15;
    unsigned char unk16, unk17;
    unsigned char first18, second19, unk1A;
} State;
extern State D_80165960;
void func_8005CC34(s32, s32, s32, s32, s32, s32);
void func_8005DB6C(void);
void func_8005D974(s32 x, s32 y, s32 x0, s32 y0, s32 first, s32 second){
    State *s = &D_80165960;
    if (first != -1) {
        func_8005CC34(0, 0x127, x, y, 0, first);
        if (first != second) func_8005CC34(1, 0x127, x, y, 1, second);
    }
    s->x0 = x0;
    s->y0 = y0;
    s->x1 = x;
    s->y1 = y;
    s->x2 = x;
    s->y2 = y;
    s->unkC = 0;
    s->unkE = 0;
    s->unkF = 0;
    s->unk10 = 0;
    s->unk11 = 0;
    s->unk12 = 0;
    s->unk13 = 0;
    s->unk16 = 0;
    s->unk17 = 0;
    s->first18 = first;
    s->second19 = second;
    s->unk1A = 0xFF;
    s->flags14 |= 1;
    func_8005DB6C();
}
