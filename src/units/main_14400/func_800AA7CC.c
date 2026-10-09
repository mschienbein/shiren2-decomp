#include "common.h"

typedef struct {
    unsigned char field_00;
    unsigned char field_01;
    unsigned short weight_02;
} Entry;
extern unsigned short D_80142B14;
extern Entry D_80142E88[];
extern s32 D_80147620[];
extern unsigned short func_800C58DC(void *, unsigned short);

void func_800AA7CC(unsigned char *first, unsigned char *second) {
    *first = 0;
    if (D_80142B14 != 0) {
        unsigned short choice = func_800C58DC(D_80147620, D_80142B14 - 1);
        Entry *entry = D_80142E88;
        while (entry->weight_02 != 0) {
            if (choice < (u32)entry->weight_02) {
                *first = entry->field_00;
                *second = entry->field_01;
                break;
            }
            choice -= entry->weight_02;
            entry++;
        }
    }
}
