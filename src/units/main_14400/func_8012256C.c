#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;
static inline unsigned char selection_index(const SelectionSave *record) { return record->index; }

static inline unsigned char selection_count(const SelectionSave *record) { return record->count; }


typedef unsigned char u8;



extern u8 D_80148780[3][3]; /* Three three-byte history records. */
extern u8 func_800A9958(void);

void func_8012256C(void) {
    u8 *out = D_80148780[0];

    out[0] = selection_index(&D_80142F24);
    out[1] = func_800A9958();
    out[2] = selection_count(&D_80142F24);
}
