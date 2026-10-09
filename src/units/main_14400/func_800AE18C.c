#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct { s32 x, y; } Pos;
typedef struct { u8 pad0[8]; s16 delta; u8 padA[2]; void (*destroy)(void *self, s32 flags); } VTable800AE18C;
typedef struct { u8 kind; u8 field_1; u8 pad2[6]; VTable800AE18C *vtable; } Obj800AE18C;

extern u8 D_80147620[];
u8 func_800AA928(void);
s32 func_800C587C(void *rng, u8 limit);
void *func_800A86EC(u8 kind);
void func_800F4830(void *item, Pos *pos);
void func_800AD7E0(Obj800AE18C *obj, void *pos, s32 notify);
u32 func_800B1C6C(void *pos);
void func_80112E7C(Obj800AE18C *obj);

/* Drops an object at pos: kind 10 may shatter into item 0x4F + field_1 (and is destroyed either
   way); everything else is placed, and a kind-2 object landing on 0x2000 terrain is handled. */
s32 func_800AE18C(Obj800AE18C *obj, Pos *pos) {
    s32 flag;
    void *item;

    if (obj->kind == 10) {
        /* ODD_C: the block groups the shatter attempt; either failed step breaks out to the
           plain destruction below. The loop scope also shapes register allocation. */
        do {
            if (func_800C587C(D_80147620, 100 - func_800AA928()) == 0) break;
            item = func_800A86EC(obj->field_1 + 0x4F);
            if (item == 0) break;
            func_800F4830(item, pos);
            if (obj != 0) {
                obj->vtable->destroy((u8 *)obj + obj->vtable->delta, 3);
            }
            return 1;
        } while (0);
        if (obj != 0) {
            obj->vtable->destroy((u8 *)obj + obj->vtable->delta, 3);
        }
        return 0;
    }
    func_800AD7E0(obj, pos, 0);
    flag = 0;
    if (func_800B1C6C(pos) & 0x2000) {
        flag = obj->kind == 2;
    }
    if (flag) {
        func_80112E7C(obj);
    }
    return 1;
}
