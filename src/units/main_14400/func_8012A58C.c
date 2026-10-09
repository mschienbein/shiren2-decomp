#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
typedef struct {
    u8 pad0[0x44]; s32 id44; u8 pad48[0x68]; s16 valueB0;
    u8 padB2[0xC]; u8 flagsBE; u8 padBF[0x7D];
} Record;
extern s32 D_801CA6D4;
extern Record *D_801CA6DC;
/* The original returns a count and stores the full halfword, unlike the old void/u8 declaration. */
s32 func_8012A58C(s32 id, s16 value) {
    s32 count;
    s32 index;
    Record *record;
    if (!id) return 0;
    index = 0;
    record = D_801CA6DC;
    count = 0;
    if (D_801CA6D4 > 0) {
        do {
            index++;
            if (record->id44 == id) {
                count++;
                record->valueB0 = value;
                record->flagsBE = 0xFF;
            }
            record++;
        } while (index < D_801CA6D4);
    }
    return count;
}
