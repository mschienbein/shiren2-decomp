#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x, y; } Pos;
/* g++ vtable entry: this-adjust delta then method. */
typedef struct { short delta; short index; void (*pfn)(void); } VEntry;
typedef struct Menu {
    unsigned char pad_00[0x34];
    Pos cursor_34;
    unsigned char pad_3C[0xC];
    s32 timeout_48;
    VEntry *vtable_4C;
} Menu;

/* Widget-family slot contracts (scratch/omp/b4-latefix/LFWidget). */
typedef void (*MoveFn)(void *self, s32 dir);          /* +0x0C */
/* +0x34 confirm: this function is its only caller (a0 = receiver, a1 = out at
 * 0x80046D94, v0 consumed); targets func_80096084 (26 tables) and func_8009BA50
 * (D_80152800) take the same two parameters (external edits, both unused there). */
typedef s32 (*ConfirmFn)(void *self, s32 *out);
typedef s32 (*EventFn)(void *self, s32 event);        /* +0x44 */
typedef s32 (*CancelFn)(void *self);                  /* +0x4C */
typedef s32 (*ValueFn)(void *self, s32 index);        /* +0x6C */
typedef s32 (*ValueAtFn)(void *self, Pos *pos);       /* +0x7C */

#define VSLOT(menu, entry) ((char *)(menu) + (menu)->vtable_4C[entry].delta)

extern void func_80048794(Menu *menu);
extern s32 func_800957B4(Menu *menu);
extern void func_800487C4(Menu *menu);
extern s32 func_8006D44C(s32 mode, s32 time);

/* Runs the menu's input loop; stores the chosen value in *out and returns 1 when a
 * choice (or timeout) ends the menu, 0 when it is cancelled. */
s32 func_80046CB0(Menu *menu, s32 *out)
{
    s32 event;
    s32 value;
    s32 none;   /* no-event sentinel returned by func_8006D44C */

    func_80048794(menu);
    if (func_800957B4(menu)) {
        *out = ((ValueFn)menu->vtable_4C[13].pfn)(VSLOT(menu, 13), 0);
        return 1;
    }
    event = -1;
    none = event;
    for (;;) {
        if (event != none) {
            func_800487C4(menu);
        }
        event = func_8006D44C(2, menu->timeout_48);
        if (event == none) {
            if (menu->timeout_48 > 0) {
                *out = 0x80000001;
                return 1;
            }
            continue;
        }
    dispatch:
        switch (event) {
        case 0x33:
            value = ((ValueAtFn)menu->vtable_4C[15].pfn)(VSLOT(menu, 15), &menu->cursor_34);
            *out = value;
            if (((ConfirmFn)menu->vtable_4C[6].pfn)(VSLOT(menu, 6), out) && value != (s32)0x80000000) {
                return 1;
            }
            break;
        case 0x38:
            ((MoveFn)menu->vtable_4C[1].pfn)(VSLOT(menu, 1), 1);
            break;
        case 0x39:
            ((MoveFn)menu->vtable_4C[1].pfn)(VSLOT(menu, 1), 0);
            break;
        case 0x3A:
            ((MoveFn)menu->vtable_4C[1].pfn)(VSLOT(menu, 1), 3);
            break;
        case 0x3B:
            ((MoveFn)menu->vtable_4C[1].pfn)(VSLOT(menu, 1), 2);
            break;
        case 0x35:
        case 0x36:
        case 0x37:
            event = ((EventFn)menu->vtable_4C[8].pfn)(VSLOT(menu, 8), event);
            func_800487C4(menu);
            goto dispatch;
        case 0x34:
            *out = 0;
            switch ((u8)((CancelFn)menu->vtable_4C[9].pfn)(VSLOT(menu, 9))) {
            case 2:
                break;
            case 0:
                return 1;
            default:
                return 0;
            }
            break;
        }
    }
}
