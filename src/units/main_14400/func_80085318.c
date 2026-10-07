#include "common.h"

/* Element of the 0x74-byte table at D_801BA380 (see func_80084AB4). */
typedef struct {
    unsigned char pad00[4];
    unsigned short field04;
    unsigned char pad06[0x16];
    u32 field1C;
    unsigned char pad20[0x54];
} Entry_80085318;

typedef struct {
    unsigned char pad00[4];
    unsigned short field04;
    unsigned short field06;
    unsigned short state08;
    unsigned char pad0A[4];
    unsigned short field0E;
    unsigned short field10;
    unsigned char pad12[2];
    s32 index14;
    unsigned char pad18[4];
    u32 field1C;
} Record_80085318;

extern Entry_80085318 D_801BA380[];

void func_80085318(Record_80085318 *record)
{
    Entry_80085318 *entry;

    switch (record->state08) {
    case 0:
        if (record->field10 == record->field06) {
            goto done;
        }
        record->state08 = 1;
        /* fall through */
    case 1:
        entry = &D_801BA380[record->index14];
        if (entry->field1C <= record->field1C || entry->field04 == 4) {
        done:
            record->field0E = 1;
            record->field04 = 4;
        }
        break;
    }
}
