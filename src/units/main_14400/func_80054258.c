#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
/* ODD_C: ROM writes the scroll word at record+0x10 (0x800543EC)
   and reads its low halfword at record+0x12 (0x800544EC). */
typedef union { s32 value; struct { u16 high, low; } halves; } Scroll;
typedef struct { s32 id, mode; u8 pad8[8]; Scroll scroll; s32 last, current, style; char text[4][0x100]; } TextRows;
extern u8 D_801398B0;
extern TextRows *func_80053A90(void);
extern s32 func_80054B48(void);
extern s32 func_800610BC(void);
extern s32 func_8007D388(void);
extern s32 func_800827BC(void);
extern void func_800827C8(s32 value);
extern void func_80082854(s32 first, s32 second);
extern void func_80082A20(char *);
extern s32 func_80083730(void);
extern void func_80083774(s32 idx, u16 *outA, u16 *outB);
extern void func_800837CC(s32 index, u16 value);
extern void func_800837F4(void);
void func_80054258(void) {
    u16 height;
    s32 mode;
    s32 row;
    s32 state;
    s32 selector;
    TextRows *rows = func_80053A90();
    state = func_800610BC();
    /* FAKEMATCH: both selector assignments are consumed. Reusing the local for
       the renderer mask ends CSE's equivalence for 3 before the mode switch;
       separate literal arguments keep that constant in a saved register. */
    selector = 3;
    if (state == selector) func_8007D388();
    selector = 15;
    func_800827C8(selector);
    mode = rows->mode;
    switch ((u32)mode) {
    case 0:
        func_80082854(0, 12);
        func_80082A20(rows->text[rows->current]);
        if (rows->current != rows->last) {
            if (rows->style == 4 && func_80083730()) rows->mode = 1;
            else { rows->mode = 2; rows->scroll.value = 0; }
        }
        break;
    case 1:
        func_80082854(0, 12);
        func_80082A20(rows->text[rows->current]);
        break;
    case 2:
        row = func_800827BC();
        func_80083774(row, 0, &height);
        height *= 8;
        if (rows->scroll.value < height) {
            rows->scroll.value += 4;
            if (height < rows->scroll.value) rows->scroll.value = height;
            func_800837F4();
            func_80082854(0, 12 - rows->scroll.value);
            func_80082A20(rows->text[rows->current]);
            func_80082854(0, 12 - rows->scroll.value + height);
            func_80082A20(rows->text[(rows->current + 1) % 4]);
        } else {
            rows->scroll.value = 0;
            rows->current = (rows->current + 1) % 4;
            func_80082854(0, 12);
            func_80082A20(rows->text[rows->current]);
            if (rows->current == rows->last) rows->mode = 0;
        }
        func_800837CC(row, rows->scroll.halves.low);
        break;
    case 3:
        row = func_800827BC();
        func_80083774(row, 0, &height);
        height *= 8;
        if (rows->scroll.value < height) {
            if (!D_801398B0) {
                D_801398B0 = mode;
                rows->scroll.value++;
                if (height < rows->scroll.value) rows->scroll.value = height;
            } else D_801398B0--;
            func_800837F4();
            func_80082854(0, 12 - rows->scroll.value);
            func_80082A20(rows->text[rows->current]);
            func_80082854(0, 12 - rows->scroll.value + height);
            func_80082A20(rows->text[(rows->current + 1) % 4]);
        } else {
            D_801398B0 = mode;
            rows->scroll.value = 0;
            rows->current = (rows->current + 1) % 4;
            func_80082854(0, 12);
            func_80082A20(rows->text[rows->current]);
            if (rows->current == rows->last) rows->mode = 0;
        }
        func_800837CC(row, rows->scroll.halves.low);
        break;
    }
    if (!rows->mode) func_80054B48();
}
