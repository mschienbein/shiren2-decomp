#include "common.h"
#include "item_records.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;


char *func_800ACB40(void *self);
s32 func_800CD278(void *data);
s32 func_800AD468(u32 id);
char *func_80048480(u16 textId);
char *func_800AC990(void *self);
s32 func_8005EF08(char *msg, const char *text, ...);
void *func_801147B4(Item *self);
char *func_80083D04(char *msg, char *extra);

char *func_80114BCC(Item *self, char *msg, s32 withExtra) {
    char *label;
    s32 value;
    s32 notSingle;

    if (self->field01 == 0xAC) {
        return self->field08->format34((Item *)((u8 *)self + self->field08->delta30), msg);
    }
    if (self->field01 == 0xB0) {
        return self->field08->format34((Item *)((u8 *)self + self->field08->delta30), msg);
    }
    label = func_800ACB40(self);
    /* Kind-9 callers and D_8015D938-family formatter dispatch select the pot list. */
    value = func_800CD278(&self->payload.pot.list);
    notSingle = func_800AD468(self->field01) != 1;
    if (notSingle && label != 0) {
        func_8005EF08(msg, func_80048480(0x2A5), label, value);
    } else {
        char *text = func_80048480(0x247);
        func_8005EF08(msg, text, func_800AC990(self), value);
    }
    if (withExtra) {
        char *extra = func_801147B4(self);
        if (extra != 0) {
            func_80083D04(msg, extra);
        }
    }
    return msg;
}
