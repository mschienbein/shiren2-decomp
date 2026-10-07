#include "common.h"

typedef struct {
    unsigned char field00;
    unsigned char pad01[3];
    u32 field04;
    void *field08;
    unsigned char field0C;
} Record_800B7800;

extern unsigned char D_80153B40[];
extern unsigned char D_80153B98[];
extern unsigned char D_80147620[];

unsigned char func_800C57A0(void *arg);

Record_800B7800 *func_800B7800(Record_800B7800 *record, unsigned char kind, signed char variant, s32 flag, u32 value)
{
    record->field08 = D_80153B40;
    record->field00 = kind;
    record->field08 = D_80153B98;
    record->field04 = value;
    if (flag != 0) {
        record->field00 = 0x21;
    }
    if (variant == -1) {
        variant = func_800C57A0(D_80147620) & 3;
    }
    record->field0C = variant;
    return record;
}
