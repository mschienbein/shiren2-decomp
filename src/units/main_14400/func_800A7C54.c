#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct {
    s16 delta;
    s16 pad2;
    void (*func)(void *, s32, void *);
} VtblEntry;
extern u8 D_801535DC[];
void func_800CA4A4(void *, void *);
void func_800A7C54(void *self, u8 *obj) {
    VtblEntry *vt;

    func_800CA4A4(obj, D_801535DC);
    vt = *(VtblEntry **)(obj + 0x18);
    vt[3].func(obj + vt[3].delta, 0xC, self);
}
