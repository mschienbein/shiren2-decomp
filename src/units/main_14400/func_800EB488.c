#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad0[0x88];
    s32 low_88;
    s32 money_8C;
} Obj_800EB488;

extern void *func_800E8A68(Obj_800EB488 *obj, u8 slot);
extern s32 func_8010BEC4(void *item, u8 id);
extern void func_800498E4(s32 message_id, ...);
extern s32 func_80049CB4(s32 id, ...);

void func_800EB488(Obj_800EB488 *obj, s16 amount) {
    s32 money = amount * 1000;
    s32 old = obj->money_8C;
    void *item = func_800E8A68(obj, 4);
    s32 limit = 200000;
    s32 delta;
    s32 message;
    s32 gained;

    money += old;
    if (item != 0 && (u8)func_8010BEC4(item, 0x6E)) {
        limit = 1000;
    }
    if (money > limit) {
        money = limit;
    } else if (money < 0) {
        money = 0;
    }
    obj->money_8C = money;
    delta = money - old;
    if (delta < 0) {
        message = 0x16;
        gained = 0;
        delta = -delta;
    } else if (delta > 0) {
        message = 0x15;
        gained = 1;
    } else {
        return;
    }
    if (obj->low_88 > obj->money_8C) {
        obj->low_88 = obj->money_8C;
    }
    func_800498E4(message, delta / 1000);
    func_80049CB4(0x7B, obj, gained);
}
