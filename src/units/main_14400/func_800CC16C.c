#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct {
    u8 pad0[0x18];
    s16 this_offset;
    u8 pad1A[2];
    void (*write)(void *self, s32 size, void *data);
} VTable800CC16C;
typedef struct { u8 pad0[0x14]; s32 done; VTable800CC16C *vtable; } Port800CC16C;
extern Port800CC16C *D_80147F44;
extern u8 D_80154254[];
void func_800CA088(Port800CC16C *port);
void func_800CA2C0(Port800CC16C *port);
void func_800CA0A8(Port800CC16C *port, s32 mode);
void func_800CBEBC(u8 a, u8 b);
void func_800CC16C(void) {
    u8 zero;
    s32 tries = 0x2B20;
    func_800CA088(D_80147F44);
    func_800CA2C0(D_80147F44);
    func_800CA0A8(D_80147F44, 8);
    D_80147F44->vtable->write((u8 *)D_80147F44 + D_80147F44->vtable->this_offset, 4, D_80154254);
    func_800CBEBC(0, 0);
    zero = 0;
    for (;;) {
        Port800CC16C *port;
        if (--tries == -1) {
            break;
        }
        port = D_80147F44;
        port->vtable->write((u8 *)port + port->vtable->this_offset, 1, &zero);
        if (D_80147F44->done != 0) {
            break;
        }
    }
    func_800CA2C0(D_80147F44);
}
