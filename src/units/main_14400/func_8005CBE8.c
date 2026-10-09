#include "common.h"
typedef struct { unsigned char pad_00[0x44]; s32 field_44; unsigned char pad_48[8]; } Record;
/* The pool descriptor and its two-element record store have independent bases. */
extern unsigned char D_801DE984[];
extern Record D_801DEA0C[2];
extern void func_8006E8E0(void *pool);
void func_8005CBE8(void) {
    Record *record = D_801DEA0C + 2;
    s32 index;
    func_8006E8E0(D_801DE984);
    index = 1;
    do {
        --record;
        --index;
        record->field_44 = -1;
    } while (index != -1);
}
