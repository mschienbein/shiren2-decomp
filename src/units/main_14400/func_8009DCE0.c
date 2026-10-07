#include "common.h"
typedef struct {
    short delta;
    short index;
    union {
        void (*text)(void *self, s32 index, char *buf);
        s32 (*attribute)(void *self, s32 index);
    } fn;
} VEntry;
typedef struct {
    char pad0[0x20];
    s32 unk20;
    char pad24[0x38 - 0x24];
    s32 unk38;
    char pad3C[0x4C - 0x3C];
    VEntry *vtbl;
    unsigned char unk50;
    unsigned char unk51;
    unsigned char unk52;
    unsigned char unk53;
} Menu;
void func_80048870(Menu *, s32);
void func_800487EC(Menu *, s32, s32, char *);
void func_8009DE8C(Menu *);
static inline s32 Menu_getId(Menu *m, s32 idx) { VEntry *e = &m->vtbl[12]; return e->fn.attribute((char *)m + e->delta, idx); }
static inline void Menu_getText(Menu *m, s32 idx, char *buf) { VEntry *e = &m->vtbl[11]; e->fn.text((char *)m + e->delta, idx, buf); }
static inline void drawItem(Menu *m, s32 idx, s32 col, s32 line) {
    char buf[0x200];
    func_80048870(m, Menu_getId(m, idx));
    Menu_getText(m, idx, buf);
    func_800487EC(m, col, line * (m->unk52 + 1) + 1, buf);
}
void func_8009DCE0(Menu *m) {
    s32 i, j, start;
    if (m->unk53 == 0) {
        drawItem(m, 0, 0, 0);
        return;
    }
    start = (m->unk38 / m->unk51) * (m->unk51 * m->unk50);
    for (i = 0; ; i++) {
        s32 rows;
        if (!(i < m->unk20)) break;
        for (j = 0; ; j++) {
            s32 idx;
            if (!(j < m->unk51)) break;
            idx = start + m->unk50 * j + i;
            if (idx < m->unk53) {
                drawItem(m, idx, i, j);
            }
        }
    }
    func_8009DE8C(m);
}
