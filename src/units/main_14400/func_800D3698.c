#include "common.h"

typedef unsigned char u8;
typedef signed char s8;

typedef struct {
    u8 pad0[0xC];
    s32 field_C;
    s32 field_10;
    u8 pad14[4];
} Record800D3698;

/* Two 0x18-byte records (splat labels D_80143330 and D_80143348). */
extern Record800D3698 D_80143330[2];

extern void func_800D2C34(Record800D3698 *record, s32 amount);

void func_800D3698(s32 side, s32 amount) {
    if ((u8)side < 2) {
        Record800D3698 *record = &D_80143330[0];
        if ((s8)side == 1) {
            record = &D_80143330[1];
        }
        func_800D2C34(record, amount);
    }
}
