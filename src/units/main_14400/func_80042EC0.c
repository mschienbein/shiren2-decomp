#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef struct VtEntry { short delta; short pad; void (*fn)(void *); } VtEntry;
typedef struct {
    u8 unk0;
    char pad1[3];
    s32 unk4;
    VtEntry *vtable;
    s32 unkC;
    u8 unk10;
} Menu;
typedef struct { char pad0[4]; u32 *count; char pad8[8]; u8 *cursor; } MenuState;
extern char D_80147620[];
extern u8 D_80143392;
s32 func_80066AD0(s32 index);
MenuState *func_80066B20(void);
u8 func_800C57CC(void *rng, u8 limit);
void func_80042EC0(Menu *menu, s8 first, s8 second, s8 initial, s8 skip) {
    s8 lo = first;
    s8 hi = second;
    s32 n = func_80066AD0(menu->unk0);
    MenuState *state;
    u32 count;
    u8 cursor;
    menu->unk10 = 0;
    if (menu->unk4 == 0) {
        if (first >= 0 && second < 0) {
            if (first < n) {
                menu->unk10 = first;
            }
        } else if (first < 0 && second >= 0) {
            if (second < n) {
                menu->unk10 = second;
            }
        } else {
            s32 last = n - 1;
            s32 start = 0;
            s32 range;
            if (lo >= 0 && hi >= 0) {
                if (hi < last) {
                    last = hi;
                }
                if (last - lo >= 0) {
                    start = lo;
                }
            }
            range = last - start;
            if (range != 0 && skip >= start && skip <= last) {
                range--;
            } else {
                skip = -1;
            }
            menu->unk10 = (u8)func_800C57CC(D_80147620, (u8)range) + start;
            if (skip >= 0 && menu->unk10 >= skip) {
                menu->unk10++;
            }
        }
    }
    menu->vtable[3].fn((char *)menu + menu->vtable[3].delta);
    state = func_80066B20();
    cursor = *state->cursor;
    count = *state->count;
    menu->unkC = -1;
    D_80143392 = cursor;
    if (count != 0) {
        if (menu->unk4 == 0) {
            if (initial < 0) {
                menu->unkC = (u8)func_800C57CC(D_80147620, (u8)(count - 1));
            } else if ((u32)initial < count) {
                menu->unkC = initial;
            } else {
                menu->unkC = count - 1;
            }
        } else {
            menu->unkC = 0;
        }
    }
}
