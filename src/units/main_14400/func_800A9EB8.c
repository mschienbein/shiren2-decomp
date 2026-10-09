#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[4];
    u8 level;   /* 0x04 */
    u8 dungeon; /* 0x05 */
    u8 pad6[2];
} Selection;

typedef struct { void *header, *descriptor, *table_a, *table_b, *table_c; u8 metadata[4]; } Resource;

typedef struct {
    u8 pad0[0x10];
    u8 field10;
} Current;

typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;

extern SelectionRecord D_80142F18;
extern SelectionSave D_80142F24;
extern Selection D_80156C70[];
extern Resource D_80142B1C[21];
extern Current *D_80142B10;
extern u8 D_801C5200[];

s32 func_800A99A8(void);
void func_800ABB50(void *arg0, u8 index, u8 arg2);
void func_800A9C8C(u8 a, u8 b);

/* ODD_C: byte predicate keeps the xor-style comparison of the saved mode. */
static inline u8 wrongMode(SelectionRecord *record) { return (record->mode & 0xE0) != 0x40; }

void func_800A9EB8(void) {
    s32 i;
    u8 center;
    u8 dungeon;

    if (func_800A99A8() && D_80142F24.count == D_80142F24.previous_count) {
        Selection *selected = &D_80156C70[D_80142F24.index];

        center = selected->level;
        dungeon = selected->dungeon;
        /* Search the five floors on either side of the saved one. */
        i = center - 5;
        while (i <= center + 5) {
            if (i >= 0 && i < D_80142B1C[dungeon].metadata[0]) {
                func_800ABB50(D_801C5200, dungeon, (u8)i);
                if (center == D_801C5200[0]) {
                    func_800A9C8C(dungeon, (u8)i);
                    return;
                }
            }
            i++;
        }
    } else {
        if (!wrongMode(&D_80142F18)) D_80142F24.result = D_80142B10->field10;
        func_800A9C8C(D_80142F24.index, D_80142F18.field_02);
    }
}
