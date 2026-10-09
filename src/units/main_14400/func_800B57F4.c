#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 field_0; struct { u8 x, count; } xy; u8 field_3; } Record;
typedef struct { s32 x, y; } Position;
extern const u16 D_80156A06, D_80156A02;
extern Record D_80143394[40];
extern u8 D_80143393;
extern s32 func_80049CB4(s32 id, ...);
void func_800B57F4(void) {
    s32 first = D_80156A06;
    s32 second = D_80156A02;
    Record *record = D_80143394;
    s32 i = 0;
    Position pos;
    while (1) {
        u8 count;
        if (i >= 40) break;
        count = record->xy.count;
        if ((u8)(count - 1) < 100) {
            count--;
            record->xy.count = count;
            if (count == 0 || count == first || count == second) {
                pos.y = record->field_0;
                pos.x = record->xy.x;
                func_80049CB4(0xD7, &pos);
                if (count == 0) D_80143393--;
            }
        }
        record++;
        i++;
    }
}
