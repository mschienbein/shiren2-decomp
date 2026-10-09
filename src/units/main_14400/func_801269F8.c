#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct {
    s16 delta;
    s16 pad2;
    void (*func)(void *, s32, void *);
} VtblEntry;
extern u8 D_80160474[];
void func_801163B0(u8 *, u8 *);
void func_800CA4A4(void *, void *);
void func_801269F8(u8 *self, u8 *obj) {
    VtblEntry *vt;
    u8 value;

    func_801163B0(self, obj);
    func_800CA4A4(obj, D_80160474);
    vt = *(VtblEntry **)(obj + 0x18);
    value = self[0x10];
    vt[3].func(obj + vt[3].delta, 1, &value);
}
