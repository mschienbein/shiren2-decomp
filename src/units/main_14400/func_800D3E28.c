#include "common.h"

typedef unsigned short u16;

typedef struct {
    s32 field_00;
    s32 field_04;
} Iterator800D3E28;

typedef struct Unit800D3E28 Unit800D3E28;
typedef struct Entity800D3E28 Entity800D3E28;

extern u16 D_8014767C;

extern char *func_800A3B20(Unit800D3E28 *u);
extern void func_800498E4(s32 id, ...);
extern void func_800C93F4(void);
extern void func_800D3750(void);
extern s32 func_800A9070(Iterator800D3E28 *it, s32 kind);
extern Entity800D3E28 *func_800A910C(Iterator800D3E28 *it);
extern void func_800F6220(Entity800D3E28 *entity);

void func_800D3E28(s32 kind, Unit800D3E28 *unit) {
    Iterator800D3E28 it;
    Iterator800D3E28 *iter;

    if (D_8014767C & 0xC) {
        return;
    }
    switch (kind) {
    case 1:
        func_800498E4(0x1D0, func_800A3B20(unit));
        break;
    case 2:
        func_800498E4(0x1D1, func_800A3B20(unit));
        break;
    /* ODD_C: kind 0 posts no message; the label shapes codegen: without it 35 words differ (176 vs 184 bytes). */
    case 0:
        break;
    }
    func_800C93F4();
    func_800D3750();
    it.field_00 = 0;
    iter = &it;
    while (func_800A9070(iter, 0x57)) {
        func_800F6220(func_800A910C(iter));
    }
}
