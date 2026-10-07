#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct Obj80045FF0 Obj80045FF0;

typedef struct {
    s16 delta;
    s16 index;
    void (*func)(Obj80045FF0 *self, s32 size, void *data);
} VtEntry80045FF0;

struct Obj80045FF0 {
    u8 pad0[0x18];
    VtEntry80045FF0 *vtable;
};

extern u8 D_8014A958[];
void func_800CA4A4(Obj80045FF0 *self, void *desc);
s16 *func_80045B48(void);
u8 func_80045B64(s32 id);

void func_80045FF0(Obj80045FF0 *self) {
    s16 *ids;

    func_800CA4A4(self, D_8014A958);
    ids = func_80045B48();
    for (;;) {
        s32 id = *ids;
        u8 value;

        self->vtable[3].func((Obj80045FF0 *)((u8 *)self + self->vtable[3].delta), 4, &id);
        if (id == -1) {
            break;
        }
        value = func_80045B64(id);
        self->vtable[3].func((Obj80045FF0 *)((u8 *)self + self->vtable[3].delta), 1, &value);
        ids++;
    }
}
