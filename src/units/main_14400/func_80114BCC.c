#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct Item80114BCC Item80114BCC;

typedef struct {
    s16 delta;
    s16 index;
    char *(*func)(Item80114BCC *self, char *msg);
} VtEntry80114BCC;

/* func_800CFF00 constructs this 0x1C-byte member and stores its owner at 0x18. */
typedef struct {
    const void *descriptor;
    const void *vtable;
    u8 *storage;
    u8 unkC;
    u8 unkD;
    u8 unkE[2];
    u8 inlineStorage[8];
    Item80114BCC *owner;
} Inventory80114BCC;

struct Item80114BCC {
    u8 unk0;
    u8 id;
    u8 pad2[6];
    VtEntry80114BCC *vtable;
    Inventory80114BCC inventory;
};

char *func_800ACB40(Item80114BCC *self);
s32 func_800CD278(Inventory80114BCC *data);
s32 func_800AD468(u32 id);
char *func_80048480(u16 textId);
char *func_800AC990(Item80114BCC *self);
s32 func_8005EF08(char *msg, const char *text, ...);
void *func_801147B4(Item80114BCC *self);
char *func_80083D04(char *msg, char *extra);

char *func_80114BCC(Item80114BCC *self, char *msg, s32 withExtra) {
    char *label;
    s32 value;
    s32 notSingle;

    if (self->id == 0xAC) {
        return self->vtable[6].func((Item80114BCC *)((u8 *)self + self->vtable[6].delta), msg);
    }
    if (self->id == 0xB0) {
        return self->vtable[6].func((Item80114BCC *)((u8 *)self + self->vtable[6].delta), msg);
    }
    label = func_800ACB40(self);
    value = func_800CD278(&self->inventory);
    notSingle = func_800AD468(self->id) != 1;
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
