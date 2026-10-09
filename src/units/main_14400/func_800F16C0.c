#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[2]; u8 flags_2; } Item800F1568;
typedef struct { u8 pad_0[0x72]; u8 flags_72; u8 pad_73[0x19]; void *list_8C; } Obj800F1568;
extern s32 func_800CF47C(void *);
extern Item800F1568 *func_800F1568(Obj800F1568 *obj, s32 force);
extern s32 func_800ADC90(void *obj, void *pos, void *origin);
void func_800F16C0(Obj800F1568 *self, s32 force) {
    s32 available = 0;
    if (self->list_8C == 0 || func_800CF47C(self->list_8C) == 0) available = 1;
    if (available) {
        Item800F1568 *item = func_800F1568(self, force);
        if (item != 0) {
            item->flags_2 |= 0x40;
            func_800ADC90(item, self, self);
            self->flags_72 &= 0xF7;
        }
    }
}
