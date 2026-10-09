#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Pos_800FA894;

typedef struct {
    Pos_800FA894 pos_0;
    u8 pad8[0x4E];
    u8 flag_56;
    u8 pad57[0xD];
    s32 field_64;
    s32 field_68;
    u8 pad6C[0x6];
    u8 flags_72;
    u8 pad73[0x19];
    void *field_8C;
    u8 pad90[0x2C];
    s32 field_BC;
} Obj_800FA894;

typedef struct {
    void *target_0;
} Ref_800FA894;

typedef struct {
    s32 type_0;
    u8 pad4[0xC];
    Ref_800FA894 *ref_10;
} Msg_800FA894;

typedef struct {
    u8 pad0[2];
    u8 flags_2;
} Effect_800FA894;

static inline void pos_copy(Pos_800FA894 *dst, Pos_800FA894 *src) {
    dst->x = src->x;
    dst->y = src->y;
}

extern s32 func_800A6FD0(void *obj);
extern s32 func_800CF47C(void *field);
extern Effect_800FA894 *func_800FA5FC(Obj_800FA894 *obj);
extern s32 func_800ADC90(void *effect, void *a, void *b);
extern void *func_800B4D80(Pos_800FA894 *pos);
extern s32 func_800FA710(Obj_800FA894 *obj, Pos_800FA894 *pos);
extern s32 func_800E2044(void *obj);
extern u16 func_800E08B0(void *obj);
extern s32 func_800F0EC4(void *obj);
extern s32 func_80049CB4(s32 id, ...);
extern char *func_800A3B20(void *obj);
extern char *func_800AE674(void *item);
extern void func_800497F0(s32 id, ...);
extern void func_800AD868(Pos_800FA894 *pos);
extern s32 func_800CD5C0(void *container, void *item);
extern s32 func_800A529C(void *obj, s32 flag);
extern s32 func_800F27A4(Obj_800FA894 *obj, Msg_800FA894 *msg);

s32 func_800FA894(Obj_800FA894 *obj, Msg_800FA894 *msg) {
    Pos_800FA894 pos;
    Effect_800FA894 *effect;
    void *item;
    s32 ok;
    s32 text;

    switch (msg->type_0) {
        case 8:
        case 9:
            if (msg->ref_10->target_0 != 0 && func_800A6FD0(msg->ref_10->target_0) != 0) {
                obj->field_BC = 1;
            }
            break;
        case 0xC:
            if ((func_800CF47C(obj->field_8C) ^ 1) != 0 && (obj->flags_72 & 8)) {
                effect = func_800FA5FC(obj);
                if (effect != 0) {
                    effect->flags_2 |= 0x40;
                    func_800ADC90(effect, obj, obj);
                }
            }
            obj->flags_72 &= ~8;
            return 1;
        case 2:
            if (!obj->flag_56) {
                break;
            }
            /* fallthrough */
        case 0x14:
            pos_copy(&pos, &obj->pos_0);
            item = func_800B4D80(&pos);
            ok = 0;
            if (func_800FA710(obj, &pos) != 0 && func_800E2044(obj) != 0 && func_800E08B0(obj) != 0) {
                ok = func_800F0EC4(obj) == 0;
            }
            if (ok) {
                text = func_80049CB4(0xDA, &pos);
                func_800497F0(0x76, text, func_800A3B20(obj), func_800AE674(item));
                func_800AD868(&pos);
                func_800CD5C0(obj->field_8C, item);
                func_800A529C(obj, 1);
                func_80049CB4(0x132);
                obj->field_64 = 0;
                obj->field_68 = 0;
                return 1;
            }
            break;
    }
    return func_800F27A4(obj, msg);
}
