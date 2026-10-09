#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct {
    u8 pad0[0x28];
    s16 read_delta;
    s16 read_index;
    void (*read)(void *self, s32 size, void *data);
} PortVTable;

typedef struct {
    u8 pad0[0x18];
    PortVTable *vtable;
} Port;

extern u8 D_801476D0;
extern Port *D_80147F44;
extern u8 D_80147E18[];

s32 func_800CBE18(s32 size);
void func_800CB96C(u8 *packet, u8 *out);
void func_800CBC24(u8 *packet, u8 *out);

void func_800CBF00(u8 *out) {
    s32 size = 0x14;
    s32 ready;

    if (D_801476D0 < 10) {
        size = 0x90;
    }
    ready = func_800CBE18(size) == 1;
    if (ready) {
        D_80147F44->vtable->read((u8 *)D_80147F44 + D_80147F44->vtable->read_delta, size, D_80147E18);
        if (D_801476D0 < 10) {
            func_800CB96C(D_80147E18, out);
        } else {
            func_800CBC24(D_80147E18, out);
        }
        D_801476D0++;
    }
}
