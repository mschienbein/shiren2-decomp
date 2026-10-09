#include "common.h"

/* Item table +0x44 binds the byte-valued func_8010DCEC effect query. */
typedef struct {
    unsigned char unk00[0x40];
    short unk40;
    unsigned char (*unk44)(void *, s32);
} VTable;
typedef struct { s32 unk00[2]; VTable *unk08; } Object;

short func_8010D040(Object *object, short amount, s32 flags) {
    short result = amount;
    unsigned char total;
    VTable *first_table;
    VTable *second_table;
    VTable *third_table;
    if (amount <= 0) {
        return amount;
    }
    if (flags & 0x400) {
        first_table = object->unk08;
        result = amount - (amount * (first_table->unk44((unsigned char *)object + first_table->unk40, 0x5E) & 0xFF)) / 100;
    }
    if (flags & 0x800) {
        second_table = object->unk08;
        total = second_table->unk44((unsigned char *)object + second_table->unk40, 0x61);
        third_table = object->unk08;
        total += third_table->unk44((unsigned char *)object + third_table->unk40, 5);
        if ((u32)(total & 0xFF) >= 0x65) {
            total = 100;
        }
        result -= (result * (total & 0xFF)) / 100;
    }
    return result;
}
