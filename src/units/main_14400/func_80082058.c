#include "common.h"

typedef unsigned short u16;

/* Ten 0x2C-byte window records at 0x801A9080 (partial view: only the fields used here). */
typedef struct {
    u16 field_00;
    u16 level_02;
    unsigned char pad_04[0x10 - 0x04];
    u16 dirty_10;
    unsigned char pad_12[0x2C - 0x12];
} Record;

extern u16 D_8013E810;
extern s32 D_8013E814;
extern s32 D_8013E8E0;
extern u16 D_801A9058[10];
extern Record D_801A9080[10];

extern void func_80081798(void);

/* Lower the focus level by one (minimum 1) and demote the windows below the focus line. */
void func_80082058(void) {
    s32 i;

    D_8013E810--;
    if (D_8013E810 == 0) {
        D_8013E810 = 1;
    }
    for (i = 0; i < D_8013E814; i++) {
        u16 *level = &D_801A9080[D_801A9058[i]].level_02;

        if (*level >= D_8013E810) {
            *level = 0x41;
        }
        D_801A9080[D_801A9058[i]].dirty_10 = 1;
    }
    if (D_8013E814 > 0) {
        func_80081798();
        D_8013E8E0 = 1;
    }
}
