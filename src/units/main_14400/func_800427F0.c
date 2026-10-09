#include "common.h"

typedef struct {
    unsigned char pad0[0x4C];
    const void *vtable;
    unsigned char pad50[0x10];
    s32 unk60;
    void *unk64;
    unsigned char pad68[0x10];
    unsigned char unk78[0x20];
} Work;

extern unsigned char D_80138AD0[];
extern unsigned char D_80152130[];
extern unsigned char D_80151EC8[];
extern unsigned char D_80152098[];
extern const unsigned char D_80151E38[144];
extern Work *func_800953C0(Work *work);
extern void func_80096CE4(Work *work, void *target, void *src);
extern s32 func_800957C0(Work *work, void *out, s32 a2, void *a3, s32 a4);

void func_800427F0(void *arg0) {
    Work work;
    Work *self = &work;

    func_800953C0(self);
    self->vtable = D_80152130;
    work.unk60 = -1;
    work.unk64 = D_80151EC8;
    self->vtable = D_80152098;
    func_80096CE4(self, arg0, D_80138AD0);
    func_800957C0(self, work.unk78, 1, 0, 0);
    self->vtable = D_80151E38;
}
