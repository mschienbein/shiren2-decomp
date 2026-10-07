#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct {
    u8 data[0x1C];
} Slot8012B36C;

typedef struct {
    u8 pad0[8];
    void *pending;
    u8 padC[0xC9 - 0xC];
    u8 active;
} Obj8012B36C;

extern Slot8012B36C *D_801CA6D8;
void func_80130320(Slot8012B36C *slot);
void func_80130290(Slot8012B36C *slot, void *value);

void func_8012B36C(Obj8012B36C *self, s32 index) {
    if (self->active) {
        func_80130320(&D_801CA6D8[index]);
    }
    self->active = 1;
    func_80130290(&D_801CA6D8[index], self->pending);
    self->pending = 0;
}
