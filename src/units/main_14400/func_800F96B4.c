#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Pos_800F96B4;

typedef struct {
    Pos_800F96B4 pos_0;
    u8 pad8[0x6A];
    u8 flags_72;
    u8 pad73[0x19];
    void *field_8C;
} Obj_800F96B4;

typedef struct {
    s32 type_0;
} Msg_800F96B4;

typedef struct {
    u8 pad0;
    u8 kind_1;
} Item_800F96B4;

static inline void pos_copy(Pos_800F96B4 *dst, Pos_800F96B4 *src) {
    dst->x = src->x;
    dst->y = src->y;
}

extern s32 func_800CF47C(void *field);
extern void *func_800AC244(u8 id);
extern s32 func_800ADC90(void *effect, void *a, void *b);
extern s32 func_800F069C(Obj_800F96B4 *obj);
extern Item_800F96B4 *func_800B4D80(Pos_800F96B4 *pos);
extern char *func_800AE674(void *item);
extern s32 func_800F9564(Obj_800F96B4 *obj, Item_800F96B4 *item, Pos_800F96B4 *pos);
extern s32 func_80049CB4(s32 command, ...);
extern char *func_800A3B20(void *obj);
extern void func_800497F0(s32 id, ...);
extern s32 func_800A529C(void *obj, s32 flag);
extern s32 func_800F27A4(Obj_800F96B4 *obj, Msg_800F96B4 *msg);

s32 func_800F96B4(Obj_800F96B4 *obj, Msg_800F96B4 *msg) {
    Pos_800F96B4 pos;
    Item_800F96B4 *item;
    void *effect;
    char *name;
    s32 text;

    switch (msg->type_0) {
        case 0xC:
            if ((func_800CF47C(obj->field_8C) ^ 1) != 0 && (obj->flags_72 & 8)) {
                effect = func_800AC244(0xCC);
                if (effect != 0) {
                    func_800ADC90(effect, obj, obj);
                }
            }
            obj->flags_72 &= ~8;
            return 1;
        case 0x14:
            if ((func_800F069C(obj) ^ 1) != 0) {
                pos_copy(&pos, &obj->pos_0);
                item = func_800B4D80(&pos);
                if (item != 0 && item->kind_1 == 0xCC) {
                    name = func_800AE674(item);
                    if (func_800F9564(obj, item, &pos) != 0) {
                        text = func_80049CB4(0xDA, &pos);
                        func_800497F0(0x76, text, func_800A3B20(obj), name);
                        func_800A529C(obj, 1);
                    }
                }
            }
            break;
    }
    return func_800F27A4(obj, msg);
}
