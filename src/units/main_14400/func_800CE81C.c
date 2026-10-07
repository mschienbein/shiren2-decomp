#include "common.h"

/* Partial views through offset E; the full record and allocation are unknown. */
typedef struct Func800CE81CRecord {
    unsigned char unknown_0[8];
    unsigned char *buffer_8;
    unsigned char count_c;
    unsigned char unknown_d;
    unsigned char position_e;
} Func800CE81CRecord;

void func_800CE81C(Func800CE81CRecord *record, s32 index) {
    if ((u32)index < (u32)record->position_e) {
        record->position_e--;
    }

    for (; index < record->count_c - 1; index++) {
        record->buffer_8[index] = record->buffer_8[index + 1];
    }

    record->buffer_8[record->count_c - 1] = 0xFF;
}
