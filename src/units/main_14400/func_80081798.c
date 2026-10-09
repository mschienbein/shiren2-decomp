#include "common.h"

typedef unsigned short u16;

/* Ten 0x2C-byte window records at 0x801A9080 (partial view: only the fields used here). */
typedef struct {
    u16 field_00;
    u16 active_02;
    unsigned char pad_04[0x0E - 0x04];
    u16 order_0E;
    unsigned char pad_10[0x2C - 0x10];
} Record;

extern s32 D_8013E814;
extern s32 D_8013E818;
extern u16 D_801A9058[10];
extern Record D_801A9080[10];

/* Rebuild the draw-order list D_801A9058 from the active records, sorted by order_0E. */
void func_80081798(void) {
    s32 priorities[10];
    s32 i = 0;

    D_8013E818 = 0;
    D_8013E814 = 0;
    for (; i < 10; i++) {
        if (D_801A9080[i].active_02) {
            D_801A9058[D_8013E818] = i;
            priorities[D_8013E818++] = D_801A9080[i].order_0E;
        }
    }

    /* Bubble sort; one temporary serves both swaps and doubles as the "no swap" sentinel. */
    for (i = 0; i < D_8013E818 - 1; i++) {
        s32 j;
        s32 temp = 0xFFFF;

        for (j = D_8013E818 - 2; j >= i; j--) {
            if (priorities[j] > priorities[j + 1]) {
                temp = priorities[j];
                priorities[j] = priorities[j + 1];
                priorities[j + 1] = temp;
                temp = D_801A9058[j];
                D_801A9058[j] = D_801A9058[j + 1];
                D_801A9058[j + 1] = temp;
            }
        }
        if (temp == 0xFFFF) {
            break;
        }
    }

    for (i = 0; i < D_8013E818; i++) {
        if (D_801A9080[D_801A9058[i]].active_02 < 0x40) {
            D_8013E814 = i + 1;
        }
        D_801A9080[D_801A9058[i]].order_0E = i;
    }
}
