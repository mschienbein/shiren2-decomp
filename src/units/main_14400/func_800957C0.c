#include "common.h"

/* Modal menu driver: runs the menu stack D_80140234[0..D_80140254], descending into the
 * child menu chosen by each selection and unwinding on cancel. Records each selected
 * token in the global Selection at D_80140210 (entries[8] at 0x80140210, count at
 * 0x80140230; func_80095CC0 copies it into a caller's Selection). The count is a
 * member of that aggregate: its store is a struct-member store, which keeps the
 * following Menu field loads ordered after it as in the ROM. */

/* g++ 2.x vtable entry: this-adjustment, index, function. Slot contracts used here, checked
 * against every one of the 27 widget tables (slot-fix/tables.json):
 *   7  (+0x3C) s32 (self, s32 *result): func_8009607C in all tables; it ignores both
 *      arguments, but this call site loads them (ROM 0x80095A44 a1 = result,
 *      0x80095A4C..58 a0 = self + delta). See slot-fix/results.json.
 *   14 (+0x74) void *(self, s32 cell): 12 overrides, all (pointer, int) -> pointer; the
 *      root table's func_800D8F80 is the shared empty placeholder.
 *   15 (+0x7C) s32 (self, Cursor *): func_80095F90 / func_8009829C / func_8009BA48.
 *   17 (+0x8C) void *(self, Cursor): func_80095C8C / func_80098964. */
typedef struct { short delta; short index; void *fn; } VEntry;
typedef struct { s32 x; s32 y; } Cursor;
/* Selection path (func_80095CC0): tokens of each level and their count. */
typedef struct { void *entries[8]; s32 count; } Selection;
typedef struct Menu {
    char pad0[0x20];
    s32 columns;
    char pad24[0x34 - 0x24];
    Cursor cursor;
    char pad3C[0x4C - 0x3C];
    VEntry *vtbl;
} Menu;
extern Menu *D_80140234[8];
extern s32 D_80140254;
extern Selection D_80140210;
void func_80041434(s32);
void func_80045A24(s32);
void func_80095D64(Menu *);
s32 func_80049CB4(s32 id, ...);
void func_80095B84(Menu *menu, void *wanted);
s32 func_80046CB0(Menu *, s32 *out);
s32 func_800957B4(Menu *);
void func_80095D04(Menu *);

s32 func_800957C0(Menu *menu, s32 *result, s32 pause, Selection *history, s32 sound) {
    Menu *m = menu;
    Selection path;
    s32 state;
    D_80140234[0] = m;
    D_80140254 = 0;
    if (history) {
        path = *history;
        history->count = 0;
    } else {
        path.count = 0;
    }
    if (pause) func_80041434(1);
    func_80045A24(sound ? 4 : 3);
    func_80095D64(m);
    if (sound) {
        func_80049CB4(0x129, sound);
        func_80049CB4(2);
    }
    for (;;) {
        s32 depth = D_80140254;
        if (depth < path.count) {
            /* Replay the saved path: move the cursor onto the recorded token. */
            VEntry *e;
            func_80095B84(m, path.entries[depth]);
            state = 2;
            e = &m->vtbl[15];
            *result = ((s32 (*)(char *, Cursor *))e->fn)((char *)m + e->delta, &m->cursor);
        } else {
            path.count = 0;
            state = func_80046CB0(m, result) ? (func_800957B4(m) ? 2 : 1) : 0;
        }
        if (state != 0 && *result != (s32)0x80000001) {
            VEntry *e = &m->vtbl[17];
            Menu *child;
            Cursor *pos;
            s32 delta = e->delta;
            Cursor c = m->cursor;
            D_80140210.entries[D_80140254] = ((void *(*)(char *, Cursor))e->fn)((char *)m + delta, c);
            D_80140210.count = D_80140254 + 1;
            pos = &m->cursor;
            e = &m->vtbl[14];
            child = ((Menu *(*)(char *, s32))e->fn)((char *)m + e->delta, pos->x + m->columns * pos->y);
            if (child != 0) {
                m = child;
                D_80140254++;
                D_80140234[D_80140254] = m;
                result++;
                if (state != 2) func_80045A24(3);
                func_80095D64(m);
                continue;
            }
        } else {
            D_80140210.count = D_80140254;
        }
        func_80045A24(3);
        func_80095D04(m);
        /* Unwind until a level accepts the cancel (or the stack is empty); `sound` is
         * reused as the stop flag. */
        while (--D_80140254 >= 0) {
            result--;
            m = D_80140234[D_80140254];
            sound = 0;
            if (!func_800957B4(m)) {
                if (state == 0) {
                    sound = 1;
                } else {
                    VEntry *e = &m->vtbl[7];
                    if (!((s32 (*)(char *, s32 *))e->fn)((char *)m + e->delta, result)) sound = 1;
                }
            }
            if (sound) break;
            func_80095D04(m);
        }
        if (D_80140254 < 0) break;
    }
    if (pause) func_80041434(0);
    return state != 0;
}
