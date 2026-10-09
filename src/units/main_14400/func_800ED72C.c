#include "common.h"
typedef struct { unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08; signed char result; unsigned char field_0A; } SelectionSave;
extern SelectionSave D_80142F24;
static inline unsigned char selection_index(const SelectionSave *record) { return record->index; }

static inline unsigned char selection_count(const SelectionSave *record) { return record->count; }

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct {
    char pad0[0x94];
    u8 unk94;
    char pad95[0x4F];
    u16 flagsE4;
    char padE6[0x22];
    u8 timer108;
} Obj800ED72C;
typedef struct Event Event;


extern u16 D_801476BE;
extern u16 D_801476C0;
s32 func_800E4B60(Obj800ED72C *, Event *);
void func_800EBF34(Obj800ED72C *);
s32 func_800A99D0(void);
s32 func_800ED858(Obj800ED72C *);
s32 func_800E9E88(Obj800ED72C *);
void func_800E9FDC(Obj800ED72C *);
void func_800ED72C(Obj800ED72C *self, Event *event) {
    u8 timer;
    s32 doIt;
    u16 sound;
    s32 special;

    func_800E4B60(self, event);
    timer = self->timer108;
    if (timer != 0) {
        u16 flags = self->flagsE4;
        if (flags & 0x10) {
            self->flagsE4 = flags & ~0x10;
        } else if (timer == 1) {
            func_800EBF34(self);
        } else {
            self->timer108 = timer - 1;
        }
    }
    if (func_800A99D0() != 0) {
        return;
    }
    doIt = func_800ED858(self) == 1;
    if (func_800E9E88(self)) {
        doIt = 0;
    }
    if (doIt) {
        func_800E9FDC(self);
    }
    sound = 0;
    special = 0;
    if (selection_index(&D_80142F24) == 11) {
        special = D_801476BE == 20;
    }
    if (special) {
        sound = selection_count(&D_80142F24) + 0xFA4;
    } else if ((self->flagsE4 >> 5) & 1) {
        sound = 0xFA7;
        self->flagsE4 &= ~0x20;
        self->unk94 &= ~4;
    }
    if (sound != 0) {
        D_801476C0 = sound;
    }
}
