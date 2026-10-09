#include "common.h"

typedef struct VTable VTable;
/* Partial list view: +0x0 holds the primary table pointer, +0x4 the list table. */
typedef struct {
    const void *field_00;
    const VTable *field_04;
} Counter;
typedef struct {
    unsigned char pad_00[5];
    signed char field_05;
    unsigned char pad_06[6];
    Counter field_0C;
} Obj;
extern u32 func_80114B68(void *a);
extern void func_800D3698(s32 index, s32 amount);
extern s32 func_800CD114(Counter *obj, s32 delta);

void func_80114268(Obj *obj, s32 amount) {
    s32 index = obj->field_05;
    if (~index) {
        func_800D3698(index, -(func_80114B68(obj) * amount));
    }
    func_800CD114(&obj->field_0C, amount);
}
