#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Pos_800FF158;

/* +0x94 slot contract: s32 (*)(void *self, s32, s32, u8, s32). */
typedef struct {
    u8 pad0[0x90];
    short adjust_90;
    short pad_92;
    s32 (*method_94)(void *, s32, s32, u8, s32);
} VTable_800FF158;

typedef struct {
    Pos_800FF158 pos_0;
    u8 pad8[0x14];
    u16 flags_1C;
    u8 pad1E[0x6];
    VTable_800FF158 *vtable_24;
    u16 field_28;
} Obj_800FF158;

typedef struct {
    u8 pad0[4];
    s32 kind_4;
    u8 pad8[6];
    u16 flags_E;
} Payload_800FF158;

typedef struct {
    s32 type_0;
    u8 pad4[0xC];
    Payload_800FF158 *payload_10;
} Msg_800FF158;

extern u32 D_8013960C;
extern u8 D_80156A79;
extern void func_800FF05C(Obj_800FF158 *object);
extern s32 func_800F27A4(Obj_800FF158 *obj, Msg_800FF158 *msg);
extern u16 func_800E08B0(void *obj);
extern s32 func_800E1CC4(void *obj, s32 kind);
extern s32 func_800E1CD4(void *obj, s32 value);
extern void func_800A665C(void *obj, u8 *value);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800497F0(s32 id, ...);
extern s32 func_800A08D8(s32 mode, s32 key, s32 sel);
extern void func_800A00C4(Pos_800FF158 *origin, u8 percentage, void *attacker, s32 kind);

/* ODD_C: boolean helper; GCC 2.8.1 expands its result as sne (sltu rd,$zero,rs), as the original does. */
static inline u8 nonzero(u32 value) {
    return value != 0;
}

static inline void pos_copy(Pos_800FF158 *dst, Pos_800FF158 *src) {
    dst->x = src->x;
    dst->y = src->y;
}

static inline s32 dispatch(Obj_800FF158 *obj, s32 mode, s32 value, u8 flag) {
    VTable_800FF158 *table = obj->vtable_24;
    return table->method_94((u8 *)obj + table->adjust_90, mode, value, flag, 0);
}

s32 func_800FF158(Obj_800FF158 *obj, Msg_800FF158 *msg) {
    Pos_800FF158 pos;
    u8 direction;
    u16 hp;
    s32 ok;
    s32 text;

    pos_copy(&pos, &obj->pos_0);
    switch (msg->type_0) {
        case 1:
        case 25:
            if (obj->flags_1C & 2) {
                return 0;
            }
            break;
        case 2:
            func_800FF05C(obj);
            break;
        case 9:
            func_800F27A4(obj, msg);
            if (!func_800E08B0(obj)) {
                return 1;
            }
            if (msg->payload_10->kind_4 == 0xB) {
                D_8013960C <<= 1;
                dispatch(obj, 0, 2, 0xFE);
                D_8013960C >>= 1;
            }
            hp = func_800E08B0(obj);
            ok = 0;
            if (!func_800E1CC4(obj, 2)) {
                ok = func_800E1CD4(obj, 0xF) == 0;
            }
            if (ok && hp < 0xB) {
                func_800FF05C(obj);
                direction = 6;
                func_800A665C(obj, &direction);
                obj->flags_1C |= 0x200;
                text = func_80049CB4(0xDA, &pos);
                func_800497F0(0x142, text);
                func_800A08D8(0, text, 0);
                func_80049CB4(0x5A, obj);
                func_80049CB4(0x87, obj);
                obj->field_28 = 0;
                func_800A00C4(&pos, D_80156A79, obj, 3);
            } else {
                func_800FF05C(obj);
            }
            return 1;
        case 0xA:
            ok = 0;
            if ((msg->payload_10->flags_E >> 10) & 1 && !func_800E1CC4(obj, 2) && !func_800E1CD4(obj, 0xF)) {
                ok = nonzero(func_800E08B0(obj));
            }
            if (ok) {
                obj->field_28 = 0;
                func_80049CB4(0x5A, obj);
                func_80049CB4(0x87, obj);
                func_800A00C4(&pos, D_80156A79, obj, 3);
                return 1;
            }
            break;
    }
    return func_800F27A4(obj, msg);
}
