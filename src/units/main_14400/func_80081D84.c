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
extern s32 D_8013E8E0;
extern u16 D_801A9058[10];
extern Record D_801A9080[10];

extern void func_80083568(void);

/* Close window `index`: drop it from the draw-order list and renumber the windows after it. */
void func_80081D84(s32 index) {
    s32 i;

    if ((u32)index < 10 && D_801A9080[index].active_02) {
        for (i = 0; i < D_8013E818; i++) {
            if (D_801A9058[i] == index) {
                if (index < D_8013E814) {
                    D_8013E814--;
                }
                for (; i < D_8013E818 - 1; i++) {
                    D_801A9080[D_801A9058[i + 1]].order_0E--;
                    D_801A9058[i] = D_801A9058[i + 1];
                }
                D_8013E818--;
                break;
            }
        }
        D_801A9080[index].active_02 = 0;
        func_80083568();
        D_8013E8E0 = 1;
    }
}
