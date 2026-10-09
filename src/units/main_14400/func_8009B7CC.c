#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

extern u8 D_801528BC[];
extern void func_80048870(void *window, s32 flags);
extern void func_800487EC(void *window, s32 x, s32 y, void *text);
extern char *func_80048480(u16 id);
extern s32 func_800B0E80(s32 index);
extern void *func_800B0A70(u8 id, s32 arg1);
extern char *func_80083F34(void *name, s32 arg1, char *buffer);

static inline void drawMessage(void *window, s32 x, s32 y, s32 messageId) {
    func_800487EC(window, x, y, func_80048480(messageId));
}

/* Draw the two header lines, then a 4x6 grid of item names (5 text lines per row). */
void func_8009B7CC(void *window) {
    char buffer[16];
    s32 row;

    func_80048870(window, 0x38000000);
    drawMessage(window, 0, D_801528BC[0], 0x293);
    drawMessage(window, 0, D_801528BC[1], 0x296);
    func_80048870(window, 0x78000000);
    for (row = 0;; row++) {
        s32 col;

        if (row >= 4) {
            break;
        }
        col = 0;
        for (;; col++) {
            u8 id;
            s32 y;

            if (col >= 6) {
                break;
            }
            id = (u8)func_800B0E80(row * 6 + col);
            if (id == 0) {
                return;
            }
            *func_80083F34(func_800B0A70(id, 1), 4, buffer) = 0;
            /* ODD_C: the item's text line is built in two steps (row height, then the
               header line). As one expression GCC's loop pass strength-reduces it into
               a +5 induction; the ROM hoists it out of the column loop and recomputes
               it for every row, after the row*6 index base. */
            y = row * 5;
            y++;
            func_800487EC(window, col + 1, y, buffer);
        }
    }
}
