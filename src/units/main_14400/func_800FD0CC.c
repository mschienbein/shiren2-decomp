#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Pos_800FD0CC;

typedef struct {
    void *field_0;
    u32 field_4;
    u32 field_8;
    u16 field_C;
    u16 field_E;
    u8 field_10;
} Damage_800FD0CC;

typedef struct {
    u8 pad0[2];
    u8 flags_2;
    u8 pad3[2];
    signed char field_5;
} Item_800FD0CC;

/* +0x94 slot contract: s32 (*)(void *self, s32, s32, u8, s32). */
typedef struct {
    u8 pad0[0x90];
    short adjust_90;
    short pad_92;
    s32 (*method_94)(void *, s32, s32, u8, s32);
} VTable_800FD0CC;

typedef struct {
    Pos_800FD0CC pos_0;
    u8 pad8[0x14];
    u16 flags_1C;
    u8 pad1E[0x6];
    VTable_800FD0CC *vtable_24;
    u8 pad28[0x78];
    Item_800FD0CC *item_A0;
    u8 count_A4;
} Obj_800FD0CC;

typedef struct {
    s32 type_0;
} Msg_800FD0CC;

extern u32 D_8013960C;
extern void *func_800B4D80(Pos_800FD0CC *pos);
extern void func_80136910(Damage_800FD0CC *obj, void *a, u32 c, u32 b, u32 e);
extern void func_800A7B68(Obj_800FD0CC *object, Damage_800FD0CC *damage);
extern s32 func_800E20CC(void *arg0);
extern void func_800AD868(Pos_800FD0CC *pos);
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800F27A4(Obj_800FD0CC *obj, Msg_800FD0CC *msg);

static inline void pos_copy(Pos_800FD0CC *dst, Pos_800FD0CC *src) {
    dst->x = src->x;
    dst->y = src->y;
}

static inline Damage_800FD0CC *damage_clear(Damage_800FD0CC *out) {
    func_80136910(out, 0, 0, 0, 0);
    return out;
}

static inline s32 dispatch(Obj_800FD0CC *obj, s32 mode, s32 value) {
    VTable_800FD0CC *table = obj->vtable_24;
    return table->method_94((u8 *)obj + table->adjust_90, mode, value, 0, 0);
}

s32 func_800FD0CC(Obj_800FD0CC *obj, Msg_800FD0CC *msg) {
    Pos_800FD0CC pos;
    Damage_800FD0CC damage;
    Item_800FD0CC *item;
    s32 done;

    switch (msg->type_0) {
        case 0:
        case 1:
        case 8:
        case 9:
        case 0xB:
        case 0xC:
        case 0x13:
            if (obj->flags_1C & 1) {
                return 0;
            }
            break;
        case 2:
            if (!obj->count_A4) {
                break;
            }
            pos_copy(&pos, &obj->pos_0);
            if (!func_800B4D80(&pos)) {
                func_800A7B68(obj, damage_clear(&damage));
                return 1;
            }
            done = 0;
            if (--obj->count_A4 == 0 || func_800E20CC(obj) == 0) {
                done = 1;
            }
            if (done) {
                obj->count_A4 = 0;
                item = obj->item_A0 = func_800B4D80(&pos);
                if (item) {
                    func_800AD868(&pos);
                    obj->item_A0->flags_2 &= ~0x20;
                    obj->item_A0->field_5 = -1;
                    func_80049CB4(0x120, &pos);
                    D_8013960C <<= 1;
                    obj->flags_1C &= ~1;
                    func_80049CB4(0x88, obj);
                    if (func_800E20CC(obj)) {
                        func_80049CB4(0x93, obj);
                        func_80049CB4(0xA9, obj);
                    }
                    dispatch(obj, 1, 0xC);
                    dispatch(obj, 1, 1);
                    D_8013960C >>= 1;
                }
            }
            break;
    }
    return func_800F27A4(obj, msg);
}
