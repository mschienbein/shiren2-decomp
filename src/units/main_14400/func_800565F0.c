#include "common.h"
typedef struct Record {
    unsigned char pad_0[0x44]; s32 active_44; s32 field_48; unsigned char pad_4C[4]; /* bytes, unused */
    short kind_50; unsigned char pad_52[0xA]; short blocked_5C, field_5E;
} Record;
extern s32 D_8013A260;
extern Record D_801D40DC[32];
/* Six one-record update callbacks, followed by a null entry. */
extern void (*D_8013A264[7])(Record *);
void func_800565F0(void) {
    if (D_8013A260 != 0) {
        Record *record = D_801D40DC;
        Record *end = D_801D40DC + 32;
        for (; record < end; record++) {
            if (record->active_44 != -1 && record->blocked_5C == 0)
                D_8013A264[record->kind_50](record);
        }
    }
}
