#include "common.h"

typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionRecord D_80142F18;
extern SelectionSave D_80142F24;
typedef struct { char pad[0x94]; unsigned char field_94; } Obj;
typedef struct { unsigned char pad0[7]; unsigned char flags7; unsigned char pad8[0x14]; void *child1C; unsigned char tail20[0x10]; } Record;
extern Record D_80147680;
extern s32 D_80147678;
extern const unsigned char D_8015488C[8];
extern Obj *D_801476B8;
extern void func_800C7774(void);
extern void func_800C9820(s32, s32);
extern void func_800AA338(s32);
extern void func_800EC864(Obj *, unsigned char, s32);
extern void func_800AA31C(void);
extern void func_800EC848(Obj *);

static inline void *record_child(Record *record) { return record->child1C; }
static inline s32 record_flags(Record *record) { return record->flags7; }
static inline s32 save_index(SelectionSave *save) { return save->index; }
static inline s32 save_count(SelectionSave *save) { return save->count; }

void func_800C75F4(void)
{
    func_800C7774();
    if (record_child(&D_80147680)) {
        s32 skip = 0;
        if (!(record_flags(&D_80147680) & D_8015488C[6])) {
            skip = save_index(&D_80142F24) == 20;
        }
        if (!skip) {
            func_800C9820(save_index(&D_80142F24), save_count(&D_80142F24));
        }
    }
    switch (D_80147678) {
    case 10: {
        s32 enabled = 0;
        s32 clear = !save_count(&D_80142F24) || ((D_801476B8->field_94 >> 3) & 1);
        if (clear) {
            enabled = 1;
            D_801476B8->field_94 &= 0xF7;
        }
        func_800AA338(enabled);
        func_800EC864(D_801476B8, save_index(&D_80142F24), enabled);
        if (D_80142F18.mode == 0x6A) {
            D_801476B8->field_94 |= 8;
        } else {
            D_801476B8->field_94 &= 0xF7;
        }
        break;
    }
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        func_800AA31C();
        func_800EC848(D_801476B8);
        break;
    case 12:
        /* Result 12 needs no follow-up (its empty case is still a node of the compare tree). */
        break;
    }
}
